// fft.cpp - implementations. See fft.hpp for the API and conventions.
#include "fft.hpp"

#include <cmath>
#include <stdexcept>
#include <string>
#include <utility>

namespace fft {

namespace {
constexpr double PI = 3.14159265358979323846;

void require_power_of_two(std::size_t n, const char* who) {
    if (!is_power_of_two(n))
        throw std::invalid_argument(std::string(who) + ": N must be a power of two");
}

// Reorders a so that a[i] and a[rev(i)] are swapped, where rev(i) reverses
// the log2(N) bits of i. After this, the butterflies can work in place:
// each stage combines neighbouring blocks that are already in the right spot.
void bit_reverse_permute(Signal& a) {
    const std::size_t n = a.size();
    for (std::size_t i = 1, j = 0; i < n; ++i) {
        // Increment j as a "reversed" binary counter: propagate the carry
        // from the most significant bit downwards.
        std::size_t bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) std::swap(a[i], a[j]);  // swap each pair only once
    }
}
}  // namespace

bool is_power_of_two(std::size_t n) { return n != 0 && (n & (n - 1)) == 0; }

std::size_t next_power_of_two(std::size_t n) {
    std::size_t p = 1;
    while (p < n) p <<= 1;
    return p;
}

// ---------------------------------------------------------------------------
// Naive DFT: N outputs, each a sum of N terms -> Theta(N^2) operations.
// Fair baseline: exp(-2*pi*i*k*j/N) only takes N distinct values, so they are
// precomputed once (N trig calls instead of N^2). The table index
// (k*j) mod N is updated incrementally, without multiplication or division.
// ---------------------------------------------------------------------------
Signal dft_naive(const Signal& x) {
    const std::size_t n = x.size();
    std::vector<cd> w(n);
    for (std::size_t m = 0; m < n; ++m)
        w[m] = std::polar(1.0, -2.0 * PI * static_cast<double>(m) / n);

    Signal X(n);
    for (std::size_t k = 0; k < n; ++k) {
        cd sum(0.0, 0.0);
        std::size_t idx = 0;  // invariant: idx == (k * j) mod n
        for (std::size_t j = 0; j < n; ++j) {
            sum += x[j] * w[idx];
            idx += k;
            if (idx >= n) idx -= n;
        }
        X[k] = sum;
    }
    return X;
}

// ---------------------------------------------------------------------------
// Recursive radix-2 Cooley-Tukey (decimation in time).
//   X[k]       = E[k] + w^k O[k]
//   X[k + N/2] = E[k] - w^k O[k],   w = exp(-2*pi*i/N), k < N/2
// E, O are the DFTs of the even- and odd-indexed samples.
// Cost: two half-size problems + linear combine -> T(N) = 2T(N/2) + Theta(N).
// ---------------------------------------------------------------------------
Signal fft_recursive(const Signal& x) {
    const std::size_t n = x.size();
    require_power_of_two(n, "fft_recursive");
    if (n == 1) return x;  // the DFT of one sample is the sample itself

    const std::size_t half = n / 2;
    Signal even(half), odd(half);
    for (std::size_t i = 0; i < half; ++i) {
        even[i] = x[2 * i];
        odd[i] = x[2 * i + 1];
    }
    const Signal E = fft_recursive(even);
    const Signal O = fft_recursive(odd);

    Signal X(n);
    for (std::size_t k = 0; k < half; ++k) {
        const cd t = std::polar(1.0, -2.0 * PI * static_cast<double>(k) / n) * O[k];
        X[k] = E[k] + t;          // uses w^(k+N/2) = -w^k (symmetry)
        X[k + half] = E[k] - t;   // so one product serves two outputs
    }
    return X;
}

// ---------------------------------------------------------------------------
// Iterative in-place radix-2 FFT.
// Stage s works on blocks of size len = 2^s; every block of size len is built
// from two already-transformed halves of size len/2 with len/2 butterflies.
// log2(N) stages * N/2 butterflies -> Theta(N log N), O(1) extra memory
// (apart from the optional twiddle table).
// ---------------------------------------------------------------------------
void fft_iterative_inplace(Signal& a, Twiddle mode) {
    const std::size_t n = a.size();
    require_power_of_two(n, "fft_iterative");
    bit_reverse_permute(a);

    for (std::size_t len = 2; len <= n; len <<= 1) {
        const std::size_t half = len / 2;
        const double angle = -2.0 * PI / static_cast<double>(len);

        if (mode == Twiddle::Table) {
            std::vector<cd> w(half);
            for (std::size_t k = 0; k < half; ++k)
                w[k] = std::polar(1.0, angle * static_cast<double>(k));
            for (std::size_t start = 0; start < n; start += len) {
                for (std::size_t k = 0; k < half; ++k) {
                    const cd u = a[start + k];
                    const cd v = a[start + k + half] * w[k];
                    a[start + k] = u + v;
                    a[start + k + half] = u - v;
                }
            }
        } else {  // Twiddle::Recurrence
            const cd w_len = std::polar(1.0, angle);
            for (std::size_t start = 0; start < n; start += len) {
                cd w(1.0, 0.0);
                for (std::size_t k = 0; k < half; ++k) {
                    const cd u = a[start + k];
                    const cd v = a[start + k + half] * w;
                    a[start + k] = u + v;
                    a[start + k + half] = u - v;
                    w *= w_len;  // error in w grows with k
                }
            }
        }
    }
}

Signal fft_iterative(const Signal& x, Twiddle mode) {
    Signal a(x);
    fft_iterative_inplace(a, mode);
    return a;
}

Signal zero_pad(const Signal& x, std::size_t n) {
    if (n < x.size()) throw std::invalid_argument("zero_pad: n < x.size()");
    Signal y(x);
    y.resize(n, cd(0.0, 0.0));
    return y;
}

// ---------------------------------------------------------------------------
// Bluestein: uses k*n = (k^2 + n^2 - (k-n)^2) / 2, so
//   X[k] = c*[k] * sum_n (x[n] c*[n]) c[k-n],   c[m] = exp(+i*pi*m^2/N)
// The sum is a convolution, computed with power-of-two FFTs of size
// M >= 2N - 1 (big enough to avoid wrap-around). Cost: O(M log M) = O(N log N).
// ---------------------------------------------------------------------------
Signal bluestein(const Signal& x) {
    const std::size_t n = x.size();
    if (n <= 1) return x;
    const std::size_t m = next_power_of_two(2 * n - 1);

    // chirp[j] = exp(+i*pi*j^2/N). j^2 is reduced mod 2N, because the chirp
    // is periodic in j^2 with period 2N; this keeps the angle small.
    std::vector<cd> chirp(n);
    for (std::size_t j = 0; j < n; ++j) {
        const std::size_t j2 = (j * j) % (2 * n);
        chirp[j] = std::polar(1.0, PI * static_cast<double>(j2) / n);
    }

    Signal a(m, cd(0.0, 0.0)), b(m, cd(0.0, 0.0));
    for (std::size_t j = 0; j < n; ++j) a[j] = x[j] * std::conj(chirp[j]);
    b[0] = chirp[0];
    for (std::size_t j = 1; j < n; ++j) {
        b[j] = chirp[j];      // positive lags
        b[m - j] = chirp[j];  // negative lags, wrapped (chirp is even in j)
    }

    // Circular convolution via the convolution theorem.
    fft_iterative_inplace(a);
    fft_iterative_inplace(b);
    for (std::size_t j = 0; j < m; ++j) a[j] = std::conj(a[j] * b[j]);
    fft_iterative_inplace(a);  // conj -> fft -> conj  ==  inverse (times M)

    Signal X(n);
    for (std::size_t k = 0; k < n; ++k)
        X[k] = std::conj(chirp[k]) * std::conj(a[k]) / static_cast<double>(m);
    return X;
}

Signal fft_any(const Signal& x) {
    return is_power_of_two(x.size()) ? fft_iterative(x) : bluestein(x);
}

Signal ifft(const Signal& X) {
    const std::size_t n = X.size();
    Signal a(n);
    for (std::size_t k = 0; k < n; ++k) a[k] = std::conj(X[k]);
    a = fft_any(a);
    for (std::size_t k = 0; k < n; ++k) a[k] = std::conj(a[k]) / static_cast<double>(n);
    return a;
}

}  // namespace fft

// test_correctness.cpp - checks every transform against the naive DFT,
// against closed-form answers, and against mathematical identities.
#include "fft.hpp"

#include <cmath>
#include <cstdio>
#include <random>
#include <string>

using fft::cd;
using fft::Signal;

namespace {
int failures = 0;

// Relative L2 error ||a - b|| / ||b||: scale-independent, so one tolerance
// works for every N.
double rel_error(const Signal& a, const Signal& b) {
    double num = 0.0, den = 0.0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        num += std::norm(a[i] - b[i]);
        den += std::norm(b[i]);
    }
    return den == 0.0 ? std::sqrt(num) : std::sqrt(num / den);
}

void check(const std::string& name, double err, double tol = 1e-10) {
    const bool ok = err <= tol;
    if (!ok) ++failures;
    std::printf("[%s] %-48s err = %.2e\n", ok ? "PASS" : "FAIL", name.c_str(), err);
}

Signal random_signal(std::size_t n, std::mt19937& rng) {
    std::normal_distribution<double> d(0.0, 1.0);
    Signal x(n);
    for (auto& v : x) v = cd(d(rng), d(rng));
    return x;
}
}  // namespace

int main() {
    std::mt19937 rng(42);  // fixed seed -> reproducible tests

    std::puts("== 1. Power-of-two N: every FFT vs naive DFT ==");
    for (std::size_t n = 1; n <= 2048; n <<= 1) {
        const Signal x = random_signal(n, rng);
        const Signal ref = fft::dft_naive(x);
        const std::string s = " N=" + std::to_string(n);
        check("recursive" + s, rel_error(fft::fft_recursive(x), ref));
        check("iterative (table)" + s, rel_error(fft::fft_iterative(x, fft::Twiddle::Table), ref));
        check("iterative (recurrence)" + s,
              rel_error(fft::fft_iterative(x, fft::Twiddle::Recurrence), ref));
    }

    std::puts("\n== 2. Arbitrary N: Bluestein vs naive DFT ==");
    for (std::size_t n : {2, 3, 5, 6, 7, 12, 17, 100, 127, 1000, 1023}) {
        const Signal x = random_signal(n, rng);
        check("bluestein N=" + std::to_string(n), rel_error(fft::bluestein(x), fft::dft_naive(x)));
    }

    std::puts("\n== 3. Round trip: ifft(fft(x)) == x ==");
    for (std::size_t n : {8, 100, 1024, 1000}) {
        const Signal x = random_signal(n, rng);
        check("round trip N=" + std::to_string(n), rel_error(fft::ifft(fft::fft_any(x)), x));
    }

    std::puts("\n== 4. Closed-form cases (N=16) ==");
    {
        const std::size_t n = 16;
        Signal delta(n, cd(0, 0)); delta[0] = 1.0;           // FFT -> all ones
        check("delta -> all ones", rel_error(fft::fft_iterative(delta), Signal(n, cd(1, 0))));

        Signal ones(n, cd(1, 0)), expect(n, cd(0, 0)); expect[0] = double(n);  // FFT -> N at bin 0
        check("constant -> N at bin 0", rel_error(fft::fft_iterative(ones), expect));

        const std::size_t k0 = 3;                                 // cos at bin 3 -> N/2 at 3 and N-3
        Signal c(n), expect_c(n, cd(0, 0));
        for (std::size_t j = 0; j < n; ++j) c[j] = std::cos(2.0 * M_PI * k0 * j / n);
        expect_c[k0] = expect_c[n - k0] = n / 2.0;
        check("cos(bin 3) -> peaks at 3 and 13", rel_error(fft::fft_iterative(c), expect_c));
    }

    std::puts("\n== 5. Parseval: sum|x|^2 == (1/N) sum|X|^2 ==");
    for (std::size_t n : {64, 1000}) {
        const Signal x = random_signal(n, rng);
        const Signal X = fft::fft_any(x);
        double ex = 0, eX = 0;
        for (auto v : x) ex += std::norm(v);
        for (auto v : X) eX += std::norm(v);
        check("parseval N=" + std::to_string(n), std::abs(ex - eX / n) / ex);
    }

    std::puts("\n== 6. Zero-padding is NOT the length-N DFT (expected to differ) ==");
    {
        const Signal x = random_signal(6, rng);
        const Signal padded = fft::fft_iterative(fft::zero_pad(x, 8));  // 8 bins
        const Signal exact = fft::dft_naive(x);                        // 6 bins
        // Bin 0 (the sum of samples) must agree; the other bins live on different grids.
        check("bin 0 agrees (sum of samples)", std::abs(padded[0] - exact[0]) / std::abs(exact[0]));
        std::printf("       bin 1: padded = %+.3f%+.3fi   exact = %+.3f%+.3fi  (different frequencies)\n",
                    padded[1].real(), padded[1].imag(), exact[1].real(), exact[1].imag());
    }

    std::printf("\n%s (%d failure%s)\n", failures ? "SOME TESTS FAILED" : "ALL TESTS PASSED",
                failures, failures == 1 ? "" : "s");
    return failures ? 1 : 0;
}

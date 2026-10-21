// fft.hpp - Discrete Fourier Transform implementations for the
// "Analysis of Algorithms" project (Yachay Tech).
//
// Sign convention (same as NumPy):
//   forward: X[k] = sum_{n=0}^{N-1} x[n] * exp(-2*pi*i*k*n/N)
//   inverse: x[n] = (1/N) * sum_{k=0}^{N-1} X[k] * exp(+2*pi*i*k*n/N)
#pragma once

#include <complex>
#include <cstddef>
#include <vector>

namespace fft {

using cd = std::complex<double>;
using Signal = std::vector<cd>;

// How twiddle factors w^k are generated inside the iterative FFT.
//   Recurrence: w <- w * w_len  (cheap, but round-off accumulates)
//   Table:      w[k] = exp(-2*pi*i*k/len) computed directly (more accurate)
enum class Twiddle { Recurrence, Table };

// ---- Utilities ----
bool is_power_of_two(std::size_t n);
std::size_t next_power_of_two(std::size_t n);

// ---- Baseline: O(N^2), straight from the definition ----
Signal dft_naive(const Signal& x);

// ---- Cooley-Tukey radix-2, recursive: mirrors T(N) = 2T(N/2) + O(N) ----
// Requires N to be a power of two.
Signal fft_recursive(const Signal& x);

// ---- Cooley-Tukey radix-2, iterative and in-place ----
// Bit-reversal permutation followed by log2(N) stages of butterflies.
// Requires N to be a power of two.
void fft_iterative_inplace(Signal& a, Twiddle mode = Twiddle::Table);
Signal fft_iterative(const Signal& x, Twiddle mode = Twiddle::Table);

// ---- Arbitrary N ----
// Zero-padding: returns x extended with zeros to length n.
// NOTE: the FFT of the padded signal is NOT the length-N DFT of x;
// it samples the same underlying spectrum on a finer frequency grid.
Signal zero_pad(const Signal& x, std::size_t n);

// Bluestein (chirp-z): exact length-N DFT for any N, in O(N log N),
// by rewriting the DFT as a convolution computed with power-of-two FFTs.
Signal bluestein(const Signal& x);

// Dispatcher: radix-2 iterative FFT if N is a power of two, else Bluestein.
Signal fft_any(const Signal& x);

// Inverse transform for any N, via the conjugation trick:
//   ifft(X) = conj( fft( conj(X) ) ) / N
Signal ifft(const Signal& X);

}  // namespace fft

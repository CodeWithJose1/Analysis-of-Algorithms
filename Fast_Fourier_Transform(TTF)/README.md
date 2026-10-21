# FFT – Analysis of Algorithms Project (Yachay Tech)

Cooley–Tukey radix-2 FFT in C++17, compared against the naive O(N^2) DFT.

| File | Content |
|---|---|
| `include/fft.hpp`, `src/fft.cpp` | naive DFT, recursive FFT, iterative in-place FFT (bit reversal; two twiddle modes), zero-padding, Bluestein, inverse FFT |
| `tests/test_correctness.cpp` | checks vs naive DFT, closed-form cases, round trip, Parseval |
| `tests/export_cases.cpp` + `scripts/validate_numpy.py` | independent validation against `numpy.fft.fft` |

```bash
make test       # correctness tests
make validate   # comparison with NumPy (needs python3 + numpy)
```

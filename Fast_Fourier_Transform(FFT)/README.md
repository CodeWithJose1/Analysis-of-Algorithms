# FFT – Analysis of Algorithms Project (Yachay Tech)

Cooley–Tukey radix-2 FFT in C++17, compared against the naive O(N^2) DFT.

| Path | Content |
|---|---|
| `include/fft.hpp`, `src/fft.cpp` | naive DFT, recursive FFT, iterative in-place FFT (bit reversal; two twiddle modes), zero-padding, Bluestein, inverse FFT |
| `tests/test_correctness.cpp` | checks vs naive DFT, closed-form cases, round trip, Parseval |
| `tests/export_cases.cpp`, `scripts/validate_numpy.py` | independent validation against `numpy.fft.fft` |
| `bench/benchmark.cpp` | experiments: time vs N, non-power-of-two N, numerical error |
| `scripts/plot_results.py` | figures (PDF + PNG) and `results/summary.txt` |

```bash
make test          # correctness tests
make validate      # comparison with NumPy (python3 + numpy)
make bench-quick   # ~15 s smoke test of the experiments
make bench         # full experiments (a few minutes) -> results/*.csv
make plots         # figures/*.pdf, figures/*.png, results/summary.txt (needs matplotlib)
```

Measurement protocol: each sample repeats the transform until it lasts ≥ 50 ms,
7 samples per (method, N), median reported; fixed random seeds; `-O2`.
Machine and compiler are recorded in `results/system.txt`.

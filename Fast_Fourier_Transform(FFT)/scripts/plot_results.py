"""Turn the benchmark CSV files into figures (PDF for LaTeX, PNG for slides)
and print a summary table with the fitted log-log slopes.

Usage: python3 scripts/plot_results.py [results_dir] [figures_dir]
"""
import csv
import os
import sys
from collections import defaultdict

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

RES = sys.argv[1] if len(sys.argv) > 1 else "results"
OUT = sys.argv[2] if len(sys.argv) > 2 else "figures"
os.makedirs(OUT, exist_ok=True)

# 3.5 in = width of one column in the IEEE conference template.
plt.rcParams.update({
    "font.size": 8, "figure.figsize": (3.5, 2.6), "savefig.bbox": "tight",
    "axes.grid": True, "grid.alpha": 0.3, "legend.fontsize": 6.5, "lines.markersize": 3,
})

LABELS = {
    "naive_dft": "Naive DFT",
    "fft_recursive": "FFT recursive",
    "fft_iter_table": "FFT iterative (table)",
    "fft_iter_recurrence": "FFT iterative (recurrence)",
    "bluestein": "Bluestein",
    "exact_fft_any": "Exact DFT: radix-2 / Bluestein",
    "zero_pad_radix2": "Zero-pad + radix-2",
}
# One fixed colour per method, so a method looks the same in every figure.
COLORS = {
    "naive_dft": "C0", "fft_recursive": "C1", "fft_iter_recurrence": "C2",
    "fft_iter_table": "C3", "bluestein": "C4", "exact_fft_any": "C4", "zero_pad_radix2": "C5",
}
EPS = 2.0 ** -53  # unit round-off of IEEE double


def load(name):
    with open(os.path.join(RES, name)) as f:
        return list(csv.DictReader(f))


def series(rows, ycol, kind=None):
    d = defaultdict(list)
    for r in rows:
        if kind is None or r["kind"] == kind:
            d[r["method"]].append((int(r["N"]), float(r[ycol])))
    return {m: tuple(np.array(v) for v in zip(*sorted(pts))) for m, pts in d.items()}


def fix_axes(*axes):
    for ax in axes:
        ax.set_yscale("log", base=10)


def save(fig, name):
    fig.savefig(os.path.join(OUT, name + ".pdf"))
    fig.savefig(os.path.join(OUT, name + ".png"), dpi=220)
    plt.close(fig)
    print("  saved", name)


def loglog_slope(N, t, min_n=256):
    m = N >= min_n
    return np.polyfit(np.log2(N[m]), np.log2(t[m]), 1)[0]


summary = []

# ---------------------------------------------------------------- Figure 1
timing = series(load("timing.csv"), "median_s")
fig, ax = plt.subplots()
for m in ["naive_dft", "fft_recursive", "fft_iter_recurrence", "fft_iter_table"]:
    if m in timing:
        N, t = timing[m]
        ax.loglog(N, t, "o-", color=COLORS[m], label=LABELS[m], base=2)
# Reference curves anchored at the middle point of the measured data.
N, t = timing["naive_dft"]
i = len(N) // 2
ax.loglog(N, t[i] * (N / N[i]) ** 2, "k--", lw=0.8, label=r"$\propto N^2$", base=2)
N, t = timing["fft_iter_table"]
i = len(N) // 2
nlogn = N * np.log2(N)
ax.loglog(N, t[i] * nlogn / nlogn[i], "k:", lw=1.0, label=r"$\propto N\log_2 N$", base=2)
ax.set_xlabel("N (input size)")
ax.set_ylabel("time per transform (s)")
ax.set_title("Running time vs N")
fix_axes(ax)
ax.legend(loc="upper left")
save(fig, "fig1_runtime_loglog")

summary.append("Log-log slopes (fit for N >= 256; 2 means N^2, ~1.0-1.1 means N log N):")
for m, (N, t) in timing.items():
    summary.append(f"  {LABELS[m]:28s} slope = {loglog_slope(N, t):.3f}")

# ---------------------------------------------------------------- Figure 2
fig, (a1, a2) = plt.subplots(1, 2, figsize=(7.0, 2.6))
fig.subplots_adjust(wspace=0.35)
for m in ["fft_recursive", "fft_iter_recurrence", "fft_iter_table"]:
    N, t = timing[m]
    a1.semilogx(N, 1e9 * t / (N * np.log2(N)), "o-", color=COLORS[m], label=LABELS[m], base=2)
a1.set_xlabel("N")
a1.set_ylabel(r"time / $(N\log_2 N)$  (ns)")
a1.set_title(r"FFT: flat line $\Rightarrow$ $\Theta(N\log N)$")
a1.set_ylim(bottom=0)
a1.legend()
N, t = timing["naive_dft"]
a2.semilogx(N, 1e9 * t / N.astype(float) ** 2, "o-", color=COLORS["naive_dft"], label=LABELS["naive_dft"], base=2)
a2.set_xlabel("N")
a2.set_ylabel(r"time / $N^2$  (ns)")
a2.set_title(r"Naive DFT: flat line $\Rightarrow$ $\Theta(N^2)$")
a2.set_ylim(bottom=0)
a2.legend()
save(fig, "fig2_normalized_constants")

summary.append("\nHidden constants (median over N >= 256):")
for m in ["fft_recursive", "fft_iter_recurrence", "fft_iter_table"]:
    N, t = timing[m]
    k = N >= 256
    summary.append(f"  {LABELS[m]:28s} c = {np.median(1e9 * t[k] / (N[k] * np.log2(N[k]))):.2f} ns per N*log2(N)")
N, t = timing["naive_dft"]
k = N >= 256
summary.append(f"  {LABELS['naive_dft']:28s} c = {np.median(1e9 * t[k] / N[k].astype(float) ** 2):.2f} ns per N^2")

# ---------------------------------------------------------------- Figure 3
Nn, tn = timing["naive_dft"]
Nf, tf = timing["fft_iter_table"]
common = np.intersect1d(Nn, Nf)
speed = np.array([tn[Nn == n][0] / tf[Nf == n][0] for n in common])
theory = 2 * common / np.log2(common)  # N^2 / ((N/2) log2 N) complex multiplications
fig, ax = plt.subplots()
ax.loglog(common, speed, "o-", label="measured speed-up", base=2)
ax.loglog(common, theory, "s--", label=r"multiplication ratio $2N/\log_2 N$", base=2)
ax.set_xlabel("N")
ax.set_ylabel("naive DFT time / FFT time")
ax.set_title("Speed-up of the FFT over the naive DFT")
fix_axes(ax)
ax.legend()
save(fig, "fig3_speedup")

summary.append("\nSpeed-up (naive / iterative FFT, table):")
for n, s, th in zip(common, speed, theory):
    summary.append(f"  N={n:>6}: measured {s:9.1f}x   multiplication ratio {th:9.1f}x")

# ---------------------------------------------------------------- Figure 4
nonp = series(load("nonpow2.csv"), "median_s")
fig, ax = plt.subplots()
for m, style in [("naive_dft", "o-"), ("exact_fft_any", ".-"), ("zero_pad_radix2", ".-")]:
    if m in nonp:
        N, t = nonp[m]
        ax.loglog(N, t, style, color=COLORS[m], label=LABELS[m], base=2)
N, t = nonp["exact_fft_any"]
p2 = (N & (N - 1)) == 0
ax.loglog(N[p2], t[p2], "k*", ms=6, label="N power of two", base=2)
ax.set_xlabel("N")
ax.set_ylabel("time per transform (s)")
ax.set_title("Arbitrary N: staircase effect")
fix_axes(ax)
ax.legend(loc="upper left")
save(fig, "fig4_non_power_of_two")

N, t = nonp["exact_fft_any"]
pow2 = {n: tt for n, tt in zip(N, t) if (n & (n - 1)) == 0}
ratios = [tt / pow2[1 << int(np.floor(np.log2(n)))] for n, tt in zip(N, t)
          if (n & (n - 1)) != 0 and (1 << int(np.floor(np.log2(n)))) in pow2]
if ratios:
    summary.append(f"\nBluestein vs previous power of two: median slowdown {np.median(ratios):.1f}x "
                   f"(range {min(ratios):.1f}x - {max(ratios):.1f}x)")

# ---------------------------------------------------------------- Figure 5
err_rows = load("error.csv")
fwd = series(err_rows, "mean_error", kind="forward")
rt = series(err_rows, "mean_error", kind="roundtrip")
fig, (a1, a2) = plt.subplots(1, 2, figsize=(7.0, 2.6))
fig.subplots_adjust(wspace=0.35)
for m in ["naive_dft", "fft_recursive", "fft_iter_recurrence", "fft_iter_table", "bluestein"]:
    if m in fwd:
        N, e = fwd[m]
        a1.loglog(N, e, "o-", color=COLORS[m], label=LABELS[m], base=2)
N = fwd["fft_iter_table"][0]
a1.loglog(N, EPS * np.sqrt(N), "k--", lw=0.8, label=r"$\varepsilon\sqrt{N}$", base=2)
a1.loglog(N, EPS * np.log2(N), "k:", lw=1.0, label=r"$\varepsilon\log_2 N$", base=2)
a1.set_xlabel("N")
a1.set_ylabel("relative L2 error")
a1.set_title("Forward error vs long-double reference")
a1.legend()
for m in ["fft_iter_recurrence", "fft_iter_table"]:
    N, e = rt[m]
    a2.loglog(N, e, "o-", color=COLORS[m], label=LABELS[m], base=2)
N = rt["fft_iter_table"][0]
a2.loglog(N, EPS * np.log2(N), "k:", lw=1.0, label=r"$\varepsilon\log_2 N$", base=2)
a2.set_xlabel("N")
a2.set_ylabel("relative L2 error")
a2.set_title(r"Round trip: $\|\mathrm{ifft}(\mathrm{fft}(x))-x\| / \|x\|$")
fix_axes(a1, a2)
a2.legend()
save(fig, "fig5_numerical_error")

summary.append("\nForward error at the largest N measured:")
for m, (N, e) in fwd.items():
    summary.append(f"  {LABELS[m]:28s} N={N[-1]:>6}: {e[-1]:.2e}")
summary.append("Round-trip error at the largest N measured:")
for m, (N, e) in rt.items():
    summary.append(f"  {LABELS[m]:28s} N={N[-1]:>8}: {e[-1]:.2e}")

text = "\n".join(summary)
print("\n" + text)
with open(os.path.join(RES, "summary.txt"), "w") as f:
    f.write(text + "\n")

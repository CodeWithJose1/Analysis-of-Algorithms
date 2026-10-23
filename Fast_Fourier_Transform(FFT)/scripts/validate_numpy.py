"""Compare our C++ FFT outputs (CSV files from export_cases) with numpy.fft.fft.

Python is used only as an independent reference and for analysis;
all algorithms under study are implemented in C++.
"""
import glob
import sys

import numpy as np

data_dir = sys.argv[1] if len(sys.argv) > 1 else "data"
worst = 0.0
for path in sorted(glob.glob(f"{data_dir}/case_*.csv"), key=lambda p: int(p.split("_")[-1][:-4])):
    a = np.loadtxt(path, delimiter=",")
    x = a[:, 1] + 1j * a[:, 2]
    ours = a[:, 3] + 1j * a[:, 4]
    ref = np.fft.fft(x)
    err = np.linalg.norm(ours - ref) / np.linalg.norm(ref)
    worst = max(worst, err)
    kind = "radix-2" if (len(x) & (len(x) - 1)) == 0 else "Bluestein"
    print(f"N={len(x):>6}  ({kind:9})  relative L2 error vs NumPy = {err:.2e}")

print("PASS" if worst < 1e-10 else "FAIL", f"(worst = {worst:.2e})")
sys.exit(0 if worst < 1e-10 else 1)

// export_cases.cpp - writes random inputs and our FFT outputs to CSV so that
// scripts/validate_numpy.py can compare them against numpy.fft.fft.
// CSV row format: index,re_in,im_in,re_out,im_out
#include "fft.hpp"

#include <cstdio>
#include <random>
#include <string>

int main(int argc, char** argv) {
    const std::string dir = argc > 1 ? argv[1] : "data";
    std::mt19937 rng(7);
    std::normal_distribution<double> d(0.0, 1.0);

    for (std::size_t n : {16, 1024, 65536, 100, 1000, 30000}) {
        fft::Signal x(n);
        for (auto& v : x) v = fft::cd(d(rng), d(rng));
        const fft::Signal X = fft::fft_any(x);

        const std::string path = dir + "/case_" + std::to_string(n) + ".csv";
        FILE* f = std::fopen(path.c_str(), "w");
        if (!f) { std::perror(path.c_str()); return 1; }
        for (std::size_t i = 0; i < n; ++i)
            std::fprintf(f, "%zu,%.17g,%.17g,%.17g,%.17g\n", i, x[i].real(), x[i].imag(),
                         X[i].real(), X[i].imag());
        std::fclose(f);
        std::printf("wrote %s\n", path.c_str());
    }
    return 0;
}

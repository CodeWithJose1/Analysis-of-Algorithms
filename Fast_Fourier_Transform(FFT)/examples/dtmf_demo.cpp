// dtmf_demo.cpp - preliminary application: decoding telephone keys (DTMF)
// with our FFT.
//
// A DTMF key is the sum of two sinusoids: one "row" frequency
// (697, 770, 852, 941 Hz) and one "column" frequency (1209, 1336, 1477, 1633 Hz).
// Decoding = find the strongest FFT peak in each group, map to the nearest
// standard frequency, and look up the key.
//
// Outputs (in <outdir>):
//   dtmf_spectrum.csv  time samples and magnitude spectrum of key '5' at 10 dB SNR
//   dtmf_accuracy.csv  decoding accuracy for all 16 keys vs signal-to-noise ratio
#include "fft.hpp"

#include <cmath>
#include <cstdio>
#include <random>
#include <string>
#include <vector>

namespace {
constexpr double PI = 3.14159265358979323846;
constexpr double FS = 8000.0;   // sampling rate of telephony (Hz)
constexpr std::size_t N = 1024; // 128 ms window, resolution FS/N = 7.8 Hz

const double ROWS[4] = {697, 770, 852, 941};
const double COLS[4] = {1209, 1336, 1477, 1633};
const char KEYS[4][5] = {"123A", "456B", "789C", "*0#D"};

// Two unit-amplitude tones with random phases plus white Gaussian noise.
// Signal power = 1/2 + 1/2 = 1, so the noise variance is 10^(-SNR/10).
std::vector<double> make_tone(int r, int c, double snr_db, std::mt19937& rng) {
    std::uniform_real_distribution<double> phase(0.0, 2.0 * PI);
    std::normal_distribution<double> noise(0.0, std::sqrt(std::pow(10.0, -snr_db / 10.0)));
    const double p1 = phase(rng), p2 = phase(rng);
    std::vector<double> s(N);
    for (std::size_t n = 0; n < N; ++n) {
        const double t = n / FS;
        s[n] = std::sin(2 * PI * ROWS[r] * t + p1) + std::sin(2 * PI * COLS[c] * t + p2) + noise(rng);
    }
    return s;
}

// Magnitude spectrum with a Hann window (reduces spectral leakage, because the
// DTMF frequencies do not fall exactly on FFT bins).
std::vector<double> spectrum(const std::vector<double>& s) {
    fft::Signal x(N);
    for (std::size_t n = 0; n < N; ++n)
        x[n] = s[n] * (0.5 - 0.5 * std::cos(2 * PI * n / (N - 1)));
    fft::fft_iterative_inplace(x);
    std::vector<double> mag(N / 2);
    for (std::size_t k = 0; k < N / 2; ++k) mag[k] = std::abs(x[k]);
    return mag;
}

// Index (0..3) of the standard frequency nearest to the strongest bin in [lo, hi] Hz.
int strongest(const std::vector<double>& mag, double lo, double hi, const double* table) {
    const std::size_t k_lo = std::size_t(lo * N / FS), k_hi = std::size_t(hi * N / FS);
    std::size_t best = k_lo;
    for (std::size_t k = k_lo + 1; k <= k_hi; ++k)
        if (mag[k] > mag[best]) best = k;
    const double f = best * FS / N;
    int idx = 0;
    for (int i = 1; i < 4; ++i)
        if (std::abs(table[i] - f) < std::abs(table[idx] - f)) idx = i;
    return idx;
}

char decode(const std::vector<double>& s) {
    const auto mag = spectrum(s);
    return KEYS[strongest(mag, 650, 990, ROWS)][strongest(mag, 1150, 1700, COLS)];
}
}  // namespace

int main(int argc, char** argv) {
    const std::string dir = argc > 1 ? argv[1] : "results";
    std::mt19937 rng(5);

    // (1) One example for the figure: key '5' (770 Hz + 1336 Hz) at 10 dB SNR.
    const auto s = make_tone(1, 1, 10.0, rng);
    const auto mag = spectrum(s);
    FILE* f = std::fopen((dir + "/dtmf_spectrum.csv").c_str(), "w");
    std::fprintf(f, "kind,x,y\n");
    for (std::size_t n = 0; n < 240; ++n) std::fprintf(f, "time,%.6f,%.6f\n", 1000.0 * n / FS, s[n]);
    for (std::size_t k = 0; k < N / 2; ++k) std::fprintf(f, "freq,%.3f,%.6f\n", k * FS / N, mag[k]);
    std::fclose(f);
    std::printf("key '5' at 10 dB decoded as '%c'\n", decode(s));

    // (2) Accuracy over all 16 keys for several noise levels.
    const int trials = 50;
    f = std::fopen((dir + "/dtmf_accuracy.csv").c_str(), "w");
    std::fprintf(f, "snr_db,correct,total\n");
    for (double snr : {20.0, 10.0, 0.0, -5.0, -10.0, -15.0, -20.0}) {
        int ok = 0, total = 0;
        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c)
                for (int t = 0; t < trials; ++t, ++total)
                    ok += decode(make_tone(r, c, snr, rng)) == KEYS[r][c];
        std::fprintf(f, "%.0f,%d,%d\n", snr, ok, total);
        std::printf("SNR %+5.0f dB: %4d / %d keys decoded correctly (%.1f%%)\n", snr, ok, total,
                    100.0 * ok / total);
    }
    std::fclose(f);
    return 0;
}

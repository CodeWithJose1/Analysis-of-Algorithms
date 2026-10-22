// benchmark.cpp - experiments for the FFT project.
//
// Three experiments, each written to a CSV file in <outdir>:
//   timing.csv   running time vs N (powers of two) for every method
//   nonpow2.csv  running time vs arbitrary N (Bluestein vs zero-padding)
//   error.csv    numerical error vs N (forward error and round-trip error)
//
// Usage: ./benchmark [--quick] [outdir]
//
// Measurement protocol (to reduce noise):
//   * one call can be shorter than the clock resolution, so each *sample*
//     repeats the call `inner` times until it lasts >= min_sample seconds,
//     and reports total / inner;
//   * `outer` samples are taken and the MEDIAN is reported (robust to the
//     OS interrupting us), together with the minimum;
//   * the calibration loop doubles as a warm-up (caches, page faults);
//   * every result is "consumed" into a volatile sink so the compiler cannot
//     remove the computation as dead code;
//   * inputs are random with a fixed seed -> reproducible.
#include "fft.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <complex>
#include <cstdio>
#include <limits>
#include <random>
#include <string>
#include <vector>

using fft::cd;
using fft::Signal;
using Clock = std::chrono::steady_clock;

namespace {

struct Config {
    bool quick = false;
    int outer = 7;              // samples per (method, N)
    double min_sample = 0.05;   // seconds per sample
    int naive_max_exp = 14;     // naive DFT up to 2^14 (it is O(N^2)!)
    int recursive_max_exp = 20;
    int iterative_max_exp = 22;
    int nonpow2_max = 1 << 17;
    std::size_t nonpow2_naive_max = 4096;
    int forward_err_max_exp = 13;    // limited by the O(N^2) reference
    int roundtrip_err_max_exp = 22;
    int error_trials = 5;            // random signals averaged per N
};

volatile double g_sink = 0.0;
void consume(const Signal& y) { if (!y.empty()) g_sink = g_sink + y[y.size() / 2].real(); }

double seconds(Clock::duration d) { return std::chrono::duration<double>(d).count(); }

Signal random_signal(std::size_t n, std::mt19937& rng) {
    std::normal_distribution<double> d(0.0, 1.0);
    Signal x(n);
    for (auto& v : x) v = cd(d(rng), d(rng));
    return x;
}

struct Timing { double median_s, min_s; int inner; };

// Measures the time of one call f(x). Every method is timed out-of-place
// (it returns a new vector), so the O(N) allocation/copy is included equally
// for all of them; it does not change the asymptotic behaviour.
template <class F>
Timing time_it(F f, const Signal& x, const Config& cfg) {
    int inner = 1;
    for (;;) {  // calibration + warm-up
        const auto t0 = Clock::now();
        for (int i = 0; i < inner; ++i) consume(f(x));
        if (seconds(Clock::now() - t0) >= cfg.min_sample || inner >= (1 << 24)) break;
        inner *= 2;
    }
    std::vector<double> samples;
    for (int r = 0; r < cfg.outer; ++r) {
        const auto t0 = Clock::now();
        for (int i = 0; i < inner; ++i) consume(f(x));
        samples.push_back(seconds(Clock::now() - t0) / inner);
    }
    std::sort(samples.begin(), samples.end());
    return {samples[samples.size() / 2], samples.front(), inner};
}

double rel_error(const Signal& a, const Signal& b) {
    long double num = 0, den = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        num += std::norm(a[i] - b[i]);
        den += std::norm(b[i]);
    }
    return static_cast<double>(std::sqrt(num / den));
}

// ---- High-precision reference for the forward error experiment ----
using cld = std::complex<long double>;

std::vector<cld> dft_reference(const Signal& x) {
    const std::size_t n = x.size();
    const long double PI_L = 3.141592653589793238462643383279502884L;
    std::vector<cld> w(n), X(n);
    for (std::size_t m = 0; m < n; ++m)
        w[m] = std::polar(1.0L, -2.0L * PI_L * static_cast<long double>(m) / n);
    for (std::size_t k = 0; k < n; ++k) {
        cld sum(0, 0);
        std::size_t idx = 0;
        for (std::size_t j = 0; j < n; ++j) {
            sum += cld(x[j]) * w[idx];
            idx += k;
            if (idx >= n) idx -= n;
        }
        X[k] = sum;
    }
    return X;
}

double rel_error_ref(const Signal& a, const std::vector<cld>& ref) {
    long double num = 0, den = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        num += std::norm(cld(a[i]) - ref[i]);
        den += std::norm(ref[i]);
    }
    return static_cast<double>(std::sqrt(num / den));
}

// ifft(fft(x)) using the same twiddle mode in both directions.
double roundtrip_error(const Signal& x, fft::Twiddle mode) {
    Signal a = fft::fft_iterative(x, mode);
    for (auto& v : a) v = std::conj(v);
    fft::fft_iterative_inplace(a, mode);
    const double n = static_cast<double>(x.size());
    for (auto& v : a) v = std::conj(v) / n;
    return rel_error(a, x);
}

FILE* open_csv(const std::string& path, const char* header) {
    FILE* f = std::fopen(path.c_str(), "w");
    if (!f) { std::perror(path.c_str()); std::exit(1); }
    std::fprintf(f, "%s\n", header);
    return f;
}

// ---------------------------------------------------------------------------
void experiment_timing(const Config& cfg, const std::string& dir) {
    FILE* f = open_csv(dir + "/timing.csv", "method,N,median_s,min_s,inner,outer");
    std::mt19937 rng(2024);
    auto row = [&](const char* m, std::size_t n, const Timing& t) {
        std::fprintf(f, "%s,%zu,%.9e,%.9e,%d,%d\n", m, n, t.median_s, t.min_s, t.inner, cfg.outer);
        std::fflush(f);
        std::fprintf(stderr, "  %-22s N=%-8zu median=%.3e s\n", m, n, t.median_s);
    };
    for (int e = 4; e <= cfg.iterative_max_exp; ++e) {
        const std::size_t n = std::size_t{1} << e;
        const Signal x = random_signal(n, rng);
        if (e <= cfg.naive_max_exp)
            row("naive_dft", n, time_it([](const Signal& s) { return fft::dft_naive(s); }, x, cfg));
        if (e <= cfg.recursive_max_exp)
            row("fft_recursive", n, time_it([](const Signal& s) { return fft::fft_recursive(s); }, x, cfg));
        row("fft_iter_table", n, time_it([](const Signal& s) {
            return fft::fft_iterative(s, fft::Twiddle::Table); }, x, cfg));
        row("fft_iter_recurrence", n, time_it([](const Signal& s) {
            return fft::fft_iterative(s, fft::Twiddle::Recurrence); }, x, cfg));
    }
    std::fclose(f);
}

// ---------------------------------------------------------------------------
void experiment_nonpow2(const Config& cfg, const std::string& dir) {
    FILE* f = open_csv(dir + "/nonpow2.csv", "method,N,median_s,min_s");
    std::mt19937 rng(2025);

    // Geometric sweep of N plus every power of two in range, to expose the
    // "staircase" caused by rounding up to the next power of two.
    std::vector<std::size_t> sizes;
    const int points = cfg.quick ? 30 : 80;
    for (int i = 0; i < points; ++i)
        sizes.push_back(static_cast<std::size_t>(
            std::llround(256.0 * std::pow(cfg.nonpow2_max / 256.0, i / double(points - 1)))));
    for (std::size_t p = 256; p <= static_cast<std::size_t>(cfg.nonpow2_max); p <<= 1) sizes.push_back(p);
    std::sort(sizes.begin(), sizes.end());
    sizes.erase(std::unique(sizes.begin(), sizes.end()), sizes.end());

    Config c = cfg;
    c.outer = cfg.quick ? 3 : 5;
    auto row = [&](const char* m, std::size_t n, const Timing& t) {
        std::fprintf(f, "%s,%zu,%.9e,%.9e\n", m, n, t.median_s, t.min_s);
        std::fflush(f);
    };
    for (std::size_t n : sizes) {
        const Signal x = random_signal(n, rng);
        std::fprintf(stderr, "  nonpow2 N=%zu\n", n);
        // Exact length-N DFT: radix-2 if N is a power of two, Bluestein otherwise.
        row("exact_fft_any", n, time_it([](const Signal& s) { return fft::fft_any(s); }, x, c));
        // Zero-padding: faster, but computes a DIFFERENT transform (finer grid).
        row("zero_pad_radix2", n, time_it([](const Signal& s) {
            return fft::fft_iterative(fft::zero_pad(s, fft::next_power_of_two(s.size()))); }, x, c));
        if (n <= cfg.nonpow2_naive_max)
            row("naive_dft", n, time_it([](const Signal& s) { return fft::dft_naive(s); }, x, c));
    }
    std::fclose(f);
}

// ---------------------------------------------------------------------------
void experiment_error(const Config& cfg, const std::string& dir) {
    FILE* f = open_csv(dir + "/error.csv", "kind,method,N,mean_error,max_error");
    std::mt19937 rng(2026);
    auto row = [&](const char* kind, const char* m, std::size_t n, const std::vector<double>& errs) {
        double mean = 0, mx = 0;
        for (double e : errs) { mean += e; mx = std::max(mx, e); }
        mean /= errs.size();
        std::fprintf(f, "%s,%s,%zu,%.6e,%.6e\n", kind, m, n, mean, mx);
        std::fflush(f);
    };

    if (std::numeric_limits<long double>::digits <= 53)
        std::fprintf(stderr, "WARNING: long double == double on this platform; "
                             "the forward-error reference is not more precise.\n");

    // (a) Forward error against a long double reference DFT.
    for (int e = 2; e <= cfg.forward_err_max_exp; ++e) {
        const std::size_t n = std::size_t{1} << e;
        std::fprintf(stderr, "  forward error N=%zu\n", n);
        std::vector<double> naive, rec, table, recur, blue;
        for (int t = 0; t < cfg.error_trials; ++t) {
            const Signal x = random_signal(n, rng);
            const auto ref = dft_reference(x);
            naive.push_back(rel_error_ref(fft::dft_naive(x), ref));
            rec.push_back(rel_error_ref(fft::fft_recursive(x), ref));
            table.push_back(rel_error_ref(fft::fft_iterative(x, fft::Twiddle::Table), ref));
            recur.push_back(rel_error_ref(fft::fft_iterative(x, fft::Twiddle::Recurrence), ref));
            blue.push_back(rel_error_ref(fft::bluestein(x), ref));
        }
        row("forward", "naive_dft", n, naive);
        row("forward", "fft_recursive", n, rec);
        row("forward", "fft_iter_table", n, table);
        row("forward", "fft_iter_recurrence", n, recur);
        row("forward", "bluestein", n, blue);
    }

    // (b) Round-trip error ||ifft(fft(x)) - x|| / ||x||: needs no reference,
    //     so it reaches much larger N.
    for (int e = 4; e <= cfg.roundtrip_err_max_exp; ++e) {
        const std::size_t n = std::size_t{1} << e;
        std::fprintf(stderr, "  round-trip error N=%zu\n", n);
        const int trials = e > 18 ? 2 : cfg.error_trials;
        std::vector<double> table, recur;
        for (int t = 0; t < trials; ++t) {
            const Signal x = random_signal(n, rng);
            table.push_back(roundtrip_error(x, fft::Twiddle::Table));
            recur.push_back(roundtrip_error(x, fft::Twiddle::Recurrence));
        }
        row("roundtrip", "fft_iter_table", n, table);
        row("roundtrip", "fft_iter_recurrence", n, recur);
    }
    std::fclose(f);
}

}  // namespace

int main(int argc, char** argv) {
    Config cfg;
    std::string dir = "results";
    for (int i = 1; i < argc; ++i) {
        const std::string a = argv[i];
        if (a == "--quick") cfg.quick = true;
        else dir = a;
    }
    if (cfg.quick) {  // small ranges: checks that everything runs, ~1 minute
        cfg.outer = 3; cfg.min_sample = 0.01;
        cfg.naive_max_exp = 11; cfg.recursive_max_exp = 16; cfg.iterative_max_exp = 18;
        cfg.nonpow2_max = 1 << 14; cfg.nonpow2_naive_max = 1024;
        cfg.forward_err_max_exp = 10; cfg.roundtrip_err_max_exp = 16; cfg.error_trials = 3;
    }

    std::fprintf(stderr, "Experiment 1/3: timing vs N\n");
    experiment_timing(cfg, dir);
    std::fprintf(stderr, "Experiment 2/3: non-power-of-two N\n");
    experiment_nonpow2(cfg, dir);
    std::fprintf(stderr, "Experiment 3/3: numerical error\n");
    experiment_error(cfg, dir);
    std::fprintf(stderr, "Done. CSV files written to %s/ (sink=%g)\n", dir.c_str(), double(g_sink));
    return 0;
}

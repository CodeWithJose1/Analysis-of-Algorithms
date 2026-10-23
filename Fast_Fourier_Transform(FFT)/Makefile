CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
INC      := -Iinclude
BUILD    := build
LIB      := src/fft.cpp
# Optional CPU pinning to reduce noise, e.g.:  make bench PIN="taskset -c 2"
PIN      ?=

all: $(BUILD)/test_correctness $(BUILD)/export_cases $(BUILD)/benchmark $(BUILD)/dtmf_demo

$(BUILD)/%: tests/%.cpp $(LIB) include/fft.hpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(INC) $< $(LIB) -o $@

$(BUILD)/benchmark: bench/benchmark.cpp $(LIB) include/fft.hpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(INC) $< $(LIB) -o $@

$(BUILD)/dtmf_demo: examples/dtmf_demo.cpp $(LIB) include/fft.hpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(INC) $< $(LIB) -o $@

$(BUILD):
	mkdir -p $(BUILD)

test: $(BUILD)/test_correctness
	./$(BUILD)/test_correctness

validate: $(BUILD)/export_cases
	mkdir -p data
	./$(BUILD)/export_cases data
	python3 scripts/validate_numpy.py data

# Records the machine and compiler, then runs the three experiments.
system-info:
	mkdir -p results
	( (lscpu 2>/dev/null || sysctl -n machdep.cpu.brand_string); echo; \
	  $(CXX) --version | head -1; echo "CXXFLAGS=$(CXXFLAGS)"; date ) > results/system.txt

bench: $(BUILD)/benchmark system-info
	$(PIN) ./$(BUILD)/benchmark results

bench-quick: $(BUILD)/benchmark system-info
	$(PIN) ./$(BUILD)/benchmark --quick results

dtmf: $(BUILD)/dtmf_demo
	mkdir -p results
	./$(BUILD)/dtmf_demo results

plots:
	python3 scripts/plot_results.py results figures

clean:
	rm -rf $(BUILD) data

.PHONY: all test validate system-info bench bench-quick dtmf plots clean



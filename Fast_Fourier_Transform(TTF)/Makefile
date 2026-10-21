CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
INC      := -Iinclude
BUILD    := build

LIB := src/fft.cpp

all: $(BUILD)/test_correctness $(BUILD)/export_cases

$(BUILD)/%: tests/%.cpp $(LIB) include/fft.hpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(INC) $< $(LIB) -o $@

$(BUILD):
	mkdir -p $(BUILD)

test: $(BUILD)/test_correctness
	./$(BUILD)/test_correctness

validate: $(BUILD)/export_cases
	mkdir -p data
	./$(BUILD)/export_cases data
	python3 scripts/validate_numpy.py data

clean:
	rm -rf $(BUILD) data

.PHONY: all test validate clean

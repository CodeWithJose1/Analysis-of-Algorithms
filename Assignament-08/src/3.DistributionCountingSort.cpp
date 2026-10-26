/*
        Analysis of Algorithms - Assignment 08

    Name: Jose Ignacio Cabascango Fernandez

    3. Implement the DistributionCountingSort algorithm and test it. Compare it with the
    ComparisonCountingSort from previous point. What are the differences of complexity and
    implementation?
*/
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

vector<int> DistributionCountingSort(const vector<int>& input) {
    if (input.empty()) return {};

    const auto [minIt, maxIt] = minmax_element(input.begin(), input.end());
    const int minimum = *minIt;
    const int maximum = *maxIt;
    const long long range = static_cast<long long>(maximum) - minimum + 1;

    if (range > 10'000'000LL) {
        throw range_error("Key range is too large for counting sort.");
    }

    vector<int> frequency(static_cast<size_t>(range), 0);
    for (int value : input) {
        ++frequency[static_cast<size_t>(static_cast<long long>(value) - minimum)];
    }

    vector<int> output;
    output.reserve(input.size());
    for (size_t offset = 0; offset < frequency.size(); ++offset) {
        for (int occurrence = 0; occurrence < frequency[offset]; ++occurrence) {
            output.push_back(static_cast<int>(static_cast<long long>(minimum) + offset));
        }
    }
    return output;
}

int main() {
    int n;
    cout << "Number of elements: ";
    if (!(cin >> n) || n < 0) {
        cerr << "Invalid size. Enter a non-negative integer.\n";
        return 1;
    }

    vector<int> values(n);
    cout << "Enter " << n << " integers: ";
    for (int i = 0; i < n; ++i) {
        if (!(cin >> values[i])) {
            cerr << "Invalid input.\n";
            return 1;
        }
    }

    try {
        const vector<int> sorted = DistributionCountingSort(values);
        cout << "Sorted array: ";
        for (int value : sorted) cout << value << ' ';
        cout << '\n';
    } catch (const range_error& error) {
        cerr << error.what() << '\n';
        return 1;
    }
    return 0;
}
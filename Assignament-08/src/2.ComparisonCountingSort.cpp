/*
        Analysis of Algorithms - Assignment 08

    Name: Jose Ignacio Cabascango Fernandez

    2. Implement the ComparisonCountingSort algorithm and test it.
*/
#include <iostream>
#include <vector>
using namespace std;

vector<int> ComparisonCountingSort(const vector<int>& input) {
    const int n = static_cast<int>(input.size());
    vector<int> count(n, 0);
    vector<int> output(n);

    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (input[i] < input[j]) {
                ++count[j];
            } else {
                ++count[i];
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        output[count[i]] = input[i];
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

    const vector<int> sorted = ComparisonCountingSort(values);
    cout << "Sorted array: ";
    for (int value : sorted) cout << value << ' ';
    cout << '\n';
    return 0;
}
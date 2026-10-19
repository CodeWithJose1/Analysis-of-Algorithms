/*
        Analysis of Algorithms - Assignment 07

    Name: Jose Ignacio Cabascango Fernandez

    2. Design the backwards substitution algorithm and show that its running time is in .
*/
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <iomanip>
using namespace std;

using Matrix = vector<vector<double>>;
using Vector = vector<double>;

Vector backwards_substitution(const Matrix& U, const Vector& b, long long& operations) {
    int n  = static_cast<int>(U.size());
    Vector x(n, 0.0);
    operations = 0;

    if (n == 0 || b.size() != U.size()) {
        throw invalid_argument("Invalid dimensions.");
    }

    for (const auto& row : U) {
        if (row.size() != U.size()) {
            throw invalid_argument("Matrix must be square.");
        }
    }

    for (int i = n - 1; i >= 0; i--) {
        double sum = b[i];

        for (int j = i + 1; j < n; j++) {
            sum -= U[i][j] * x[j];
            operations++;
        }

        if (abs(U[i][i]) < 1e-12) {
            throw runtime_error("Matrix is singular.");
        }
        x[i] = sum / U[i][i];
    }
    return x;
}

int main() {
    Matrix U = {
        {4, 3},
        {0, -0.5}
    };

    Vector b = {11, -0.5};
    long long operations = 0;

    try {
        Vector x = backwards_substitution(U, b, operations);

        cout << fixed << setprecision(4);
        cout << "Solution:\n";

        for (double value : x) {
            cout << value << " ";
        }
        cout << "\nTotal operations: " << operations << endl;

        cout << "\nComplexity analysis:\n";
        cout << "The inner loop executes at most n(n-1)/2 times.\n";
        cout << "Therefore, backward substitution is O(n^2).\n";
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}
/*
        Analysis of Algorithms - Assignment 07

    Name: Jose Ignacio Cabascango Fernandez

    1. Implement the forward elimination algorithm for the Gaussian elimination.
*/
#include <iostream>
#include <vector>
#include <cmath> 
#include <algorithm>
#include <stdexcept>
using namespace std;    

using matrix = vector<vector<double>>;
using Vector = vector<double>;

void forward_elimination(matrix& A, Vector& b) {
    int n = static_cast<int>(A.size());

    if (n == 0 || b.size() != A.size()) {
        throw invalid_argument("Invalid dimensions for matrix A and vector b.");
    }

    for (const auto& row : A) {
        if (row.size() != A.size()) {
            throw invalid_argument("Matrix A must be square.");
        }
    }

    const double EPS = 1e-12;

    for (int k = 0; k < n - 1; k++) {
        int pivot = k;

        for (int i = k + 1; i < n; i++) {
            if (abs(A[i][k]) > abs(A[pivot][k])) {
                pivot = i;
            }
        }

        if (abs(A[pivot][k]) < EPS) {
            throw runtime_error("Matrix is singular.");
        }

        swap(A[k], A[pivot]);
        swap(b[k], b[pivot]);

        for (int i = k + 1; i < n; i++) {
            double factor = A[i][k] / A[k][k];
            A[i][k] = 0.0;

            for (int j = k + 1; j < n; j++) {
                A[i][j] -= factor * A[k][j];
            }
            b[i] -= factor * b[k];
        }
    }

    if (abs(A[n - 1][n - 1]) < EPS) {
        throw runtime_error("Matrix is singular.");
    }
}

int main() {
    matrix A = {
        {2, 1},
        {4, 3}
    };

    Vector b = {5, 11};

    try {
        forward_elimination(A, b);

        cout << "Upper triangular matrix:\n";
        
        for (const auto& row : A) {
            for (double value : row) {
                cout << value << " ";
            }
            cout << "\n";
        }

        cout << "Transformed vector b:\n";
        for (double value : b) {
            cout << value << " ";
        }
        cout << "\n";
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
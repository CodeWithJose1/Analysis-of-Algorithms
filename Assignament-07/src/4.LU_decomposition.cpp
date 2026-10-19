/*
        Analysis of Algorithms - Assignment 07

    Name: Jose Ignacio Cabascango Fernandez

    4. Implement an algorithm that can solve a system of equations using LU decomposition. LU
        decomposition has to be based on Gaussian Elimination.
*/
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <iomanip>
#include <algorithm>
using namespace std;

using Matrix = vector<vector<double>>;
using Vector = vector<double>;

void lu_decomposition(const Matrix& A, Matrix& L, Matrix& U, vector<int>& P) {
    int n = static_cast<int>(A.size());

    if (n == 0) {
        throw invalid_argument("Matrix cannot be empty.");
    }

    for (const auto& row : A) {
        if (row.size() != A.size()) {
            throw invalid_argument("Matrix must be square.");
        }
    }
    
    const double EPS = 1e-12;

    U = A;
    L.assign(n, vector(n, 0.0));
    P.resize(n);

    for (int i = 0; i < n; i++) {
        L[i][i] = 1.0;
        P[i] = i;
    }

    for (int k = 0; k < n - 1; k++) {
        int pivot = k;

        for (int i = k + 1; i < n; i++) {
            if (abs(U[i][k]) > abs(U[pivot][k])) {
                pivot = i;
            }
        }

        if (abs(U[pivot][k]) < EPS) {
            throw runtime_error("Matrix is singular.");
        }

        if (pivot != k) {
            swap(U[k], U[pivot]);
            swap(P[k], P[pivot]);
        }
    }

    for (int k = 0; k < n - 1; k++) {
        for (int i = k + 1; i < n; i++) {
            double factor = U[i][k] / U[k][k];
            L[i][k] = factor;
            U[i][k] = 0.0;

            for (int j = k + 1; j < n; j++) {
                U[i][j] -= factor * U[k][j];
            }
        }
    }
}

Vector solved_lu(const Matrix& L, const Matrix& U, const Vector& b, const vector<int>& P) {
    int n = static_cast<int>(L.size());
    Vector Pb(n), y(n, 0.0), x(n, 0.0);

    if (b.size() != L.size()) {
        throw invalid_argument("Invalid dimensions for vector b.");
    }

    for (int i = 0; i < n; i++) {
        Pb[i] = b[P[i]];
    }

    for (int i = 0; i < n; i++) {
        double sum = Pb[i];

        for (int j = 0; j < i; j++) {
            sum -= L[i][j] * y[j];
        }

        y[i] = sum;
    }

    for (int i = n - 1; i >= 0; i--) {
        double sum = y[i];

        for (int j = i + 1; j < n; j++) {
            sum -= U[i][j] * x[j];
        }

        if (abs(U[i][i]) < 1e-12) {
            throw runtime_error("Matrix is singular.");
        }
        x[i] = sum / U[i][i];
    }

    return x;
}

int main() {
    Matrix A = {
        {2, 1},
        {4, 3}
    };

    Vector b = {5, 11};
    Matrix L, U;
    vector<int> P;

    try {
        lu_decomposition(A, L, U, P);
        Vector x = solved_lu(L, U, b, P);

        cout << fixed << setprecision(4);
        cout << "Solution:\n";

        for (double value : x) {
            cout << value << " ";
        }
        cout << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}
/*
        Analysis of Algorithms - Assignment 06

    Name: Jose Ignacio Cabascango Fernandez

    1. Implement the Quick Sort algorithm and test using random generated lists.
*/
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
#include <chrono>
using namespace std;
using namespace chrono;

int hore_partition(vector<int>& A, int l, int r) {
    int p = A[l];
    int i = l;
    int j = r + 1;

    while (true) {
        do {
            i++;
        } while (i <= r && A[i] < p);

        do {
            j--;
        } while (A[j] > p);

        if (i >= j) {
            break;
        }

        swap(A[i], A[j]);
    }

    swap(A[l], A[j]);
    return j;
}

void quick_sort(vector<int>& A, int l, int r) {
    if (l < r) {
        int s = hore_partition(A, l, r);
        quick_sort(A, l, s - 1);
        quick_sort(A, s + 1, r);
    }
}

int main() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100000);
    
    vector<int> sizes = {1000, 5000, 10000, 20000, 50000};
    const int repetitions = 5;

    cout << "Repetitions: " << repetitions << endl;
    cout << left << setw(12) << "Size (n)" 
                << setw(22) << "Average time (us)" 
                << setw(22) << "Average time (ms)"
                << "n * log2(n)\n";

    cout << string(76, '-') << '\n';

    for (int n : sizes) {
        double total_microseconds = 0.0;

        for (int rep = 0; rep < repetitions; rep++) {
            vector<int> A(n);

            for (int& x : A) {
                x = dis(gen);
            }
            
            auto start = steady_clock::now();

            quick_sort(A, 0, static_cast<int>(A.size()) - 1);

            auto end = steady_clock::now();

            double elapsed =
                duration<double, micro>(end - start).count();

            total_microseconds += elapsed;

            if (!is_sorted(A.begin(), A.end())) {
                cerr << "Error: array was not sorted correctly.\n";
                return 1;
            }
        }
        
        double averageMicroseconds =
            total_microseconds / repetitions;

        double averageMilliseconds =
            averageMicroseconds / 1000.0;

        double theoreticalGrowth =
            n * log2(static_cast<double>(n));

        cout << left << setw(12) << n
                << setw(22) << fixed << setprecision(2)
                << averageMicroseconds
                << setw(22) << fixed << setprecision(4)
                << averageMilliseconds
                << fixed << setprecision(0)
                << theoreticalGrowth << '\n';
    }

    cout << "\nAll sorting tests passed.\n";

    return 0;
}









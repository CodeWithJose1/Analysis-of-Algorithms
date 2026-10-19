/*
        Analysis of Algorithms - Assignment 07

    Name: Jose Ignacio Cabascango Fernandez

    3. Implement an algorithm to transform a random array into a heap.
*/
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

void heap(vector<int>& A, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && A[left] > A[largest]) {
        largest = left;
    }

    if (right < n && A[right] > A[largest]) {
        largest = right;
    }

    if (largest != i) {
        swap(A[i], A[largest]);
        heap(A, n, largest);
    }
}

void build_heap(vector<int>& A) {
    int n = static_cast<int>(A.size());

    for (int i = n / 2 - 1; i >= 0; i--) {
        heap(A, n, i);
    }
}

bool is_heap(const vector<int>& A) {
    int n = static_cast<int>(A.size());

    for (int i = 0; i < n / 2; i++) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && A[i] < A[left]) {
            return false;
        }

        if (right < n && A[i] < A[right]) {
            return false;
        }
    }
    return true;
}

void print_array(const vector<int>& A) {
    for (int x : A) {
        cout << x << " ";
    }
    cout << endl;
}

int main() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100);

    vector<int> A(10);

    for (int& x : A) {
        x = dis(gen);
    }

    cout << "Original array:\n";
    print_array(A);

    build_heap(A);

    cout << "Max-heap:\n";
    print_array(A);

    cout << "Heap verification: " << (is_heap(A) ? "Valid heap" : "Invalid heap") << endl;

    return 0;
}
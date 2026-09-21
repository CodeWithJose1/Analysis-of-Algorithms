/*
        Analysis of Algorithms - Assignment 05

    Name: Jose Ignacio Cabascango Fernandez

    2. Generate a list of 10000000 (ten million) random numbers with C++. Use the 
    Binary Search and the Interpolation Search to find a key. Measure and report 
    the execution time for each search.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
using namespace std;
using namespace chrono;

int binary_search(const vector<int>& A, int key) {
    int left = 0;
    int right = A.size() - 1;

    while (left <= right) {
        int mid = left  + (right - left) / 2;

        if (A[mid] == key) 
            return mid;

        if (A[mid] < key)
            left = mid + 1;

        else
            right = mid - 1;
    }
    return -1;
}

int interpolation_search(const vector<int>& A, int key) {
    int left = 0;
    int right = A.size() - 1;

    while (left <= right && key >= A[left] && key <= A[right]) {
        if (left == right) {
            if (A[left] == key)
                return left;

            return -1;
        }
        
        long long pos = left + static_cast<long long>(key - A[left]) * 
                                                        (right - left) / 
                                                        (A[right] - A[left]);

        if (A[pos] == key)
            return pos;

        if (A[pos] < key)
            left = pos + 1;

        else
            right = pos - 1;
    }
    return -1;
}

int main() {
    const int n = 10000000;
    vector<int> A(n);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, 10000000);

    for (int i = 0; i < n; i++) {
        A[i] = dist(gen);
    }

    sort(A.begin(), A.end());

    int key = A[n / 2];

    cout  << "Number of elements: " << n << endl;
    cout << "Key to search: " << key << endl;

    auto start_binary = high_resolution_clock::now();
    int binary_result = binary_search(A, key);
    auto end_binary = high_resolution_clock::now();
    auto binary_duration = duration_cast<microseconds>(end_binary - start_binary).count();

    auto start_interpolation = high_resolution_clock::now();
    int interpolation_result = interpolation_search(A, key);
    auto end_interpolation = high_resolution_clock::now();
    auto interpolation_duration = duration_cast<microseconds>(end_interpolation - start_interpolation).count();

    cout << "\nBinary Search: "  << endl;
    cout << "Position: " << binary_result << endl;
    cout << "Execution time: " << binary_duration << " microseconds" << endl;

    cout << "\nInterpolation Search: " << endl;
    cout << "Position: " << interpolation_result << endl;
    cout << "Execution time: " << interpolation_duration << " microseconds" << endl;

    return 0;
}
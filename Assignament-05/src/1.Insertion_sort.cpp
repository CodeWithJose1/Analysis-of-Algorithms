/*
        Analysis of Algorithms - Assignment 05

    Name: Jose Ignacio Cabascango Fernandez

    1. Create a program that implements the Insertion Sort Algorithm. Your program 
    has to be able to sort a randomly generated array of alphabetic characters. 
    Decide if you have to do or not any adjustments for the algorithm to work 
    with alphabetic characters instead of integer numbers.
*/

#include <iostream>
#include <random>
#include <algorithm>
using namespace std;

void insertion_sort(char A[], int n) {
    for (int i = 1; i < n; i++) {
        char V = A[i];
        int j = i - 1;

        while (j >= 0 && A[j] > V) {
            A[j + 1] = A[j];
            j--;
        }
        A[j + 1] = V;
    }
}

int main() {
    const int n = 20;
    char A[n];

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist('A', 'z');

    for (int i = 0; i < n; i++) {
        A[i] = static_cast<char>(dist(gen));
    }

    cout << "Original array: ";

    for (int i = 0; i < n; i++) {
        cout << A[i] << " ";
    }

    cout << endl;
    insertion_sort(A, n);

    cout << "Sorted array: ";

    for (int i = 0; i < n; i++) {
        cout << A[i] << " ";
    }

    cout << endl;

    return 0;
}
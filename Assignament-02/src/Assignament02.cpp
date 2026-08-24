/*
        Analysis of Algorithms - Assignment 01

    Name: Jose Ignacio Cabascango Fernandez

    2. The Fibonacci sequence is generated using the following recurrence:
       T(n) = T(n-2) + T(n-1) 
*/

#include <iostream>
using namespace std;

// 2.1 Write C++ program that generates the sequence using iteration.
int iteration_fib(int n) {
    if (n == 0)
        return 0;
    
    if (n == 1)
        return 1;

    int previous = 0, current = 1;

    for (int i = 2; i <= n; i++) {
        int next = previous + current;
        previous = current;
        current = next;
    }
    return current;
}

// 2.2 Wirite C++ program that generates the sequence using recursion.
int recursion_fib(int n) {
    if (n == 0)
        return 0;

    if (n == 1)
        return 1;

    return recursion_fib(n -2) + recursion_fib(n-1);
}

int main() {
    int n;
    cout << "Enter the number of terms: ";
    cin >> n;

    cout << "Fibonacci sequence iteration: ";

    for (int i = 0; i < n; i++) {
        cout << iteration_fib(i) << " ";
    }

    cout << endl;

    cout << "Fibonacci sequence recursion: ";

    for (int i = 0; i < n; i++) {
        cout << recursion_fib(i) << " ";
    }

    cout << endl;

    return 0;
}
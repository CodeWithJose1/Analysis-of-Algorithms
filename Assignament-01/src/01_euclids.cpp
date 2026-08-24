/*
        Analysis of Algorithms - Assignment 01

    Name: Jose Ignacio Cabascango Fernandez

    1. Code Euclid’s Algorithms from the textbook.
*/

#include <iostream>
using namespace std;

int euclid(int m, int n) {
    while (n != 0) {
        int r = m % n;
        m = n;
        n = r;
    }
    return m;
}

int main() {
    int m, n;
    cout << "Enter m: ";
    cin >> m;

    cout << "Enter n: ";
    cin >> n;

    cout << "GCD = " << euclid(m, n) << endl;
    return 0;
}
/*
        Analysis of Algorithms - Assignment 08

    Name: Jose Ignacio Cabascango Fernandez

    1. Is it possible to exchange numeric values of two variables, say, u and v, without using any extra
    storage? Show how with a program.
*/
#include <iostream>
using namespace std;

int main() {
    int u, v;

    cout << "Enter integers (u and v): ";
    if (!(cin >> u >> v)) {
        cerr << "Invalid input. Please enter two integers.\n";
        return 1;
    }

    cout << "Before swap: u = " << u << ", v = " << v << '\n';

    u = u ^ v;
    v = u ^ v;
    u = u ^ v;

    cout << "After swap:  u = " << u << ", v = " << v << '\n';
    return 0;
}
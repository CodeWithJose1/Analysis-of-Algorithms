/*
        Analysis of Algorithms - Assignment 01

    Name: Jose Ignacio Cabascango Fernandez

    2. Code Consecutive integer checking algorithm from the textbook.
*/

#include <iostream>
using namespace std;

int consecutive_integer(int m, int n) {
    int t = min(m, n);
    while (true) {
        if (m % t == 0) {
            if (n % t == 0) {
                return t;
            }
        }
        t--;
    }
}

int main() {
    int m, n;

    cout << "Enter m: ";
    cin >> m;

    cout << "Enter n: ";
    cin >> n;

    cout << "GCD = " << consecutive_integer(m, n) << endl;
    return 0;
}
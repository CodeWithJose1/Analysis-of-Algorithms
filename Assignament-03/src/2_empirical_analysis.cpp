/*
        Analysis of Algorithms - Assignment 03

    Name: Jose Ignacio Cabascango Fernandez

    2.  Apply empirical analysis to the “linear congruential method” for creating pseudo-random
        numbers. Research about the algorithm, the textbook gives some information to start up with.
*/

#include <iostream>
#include <chrono>
using namespace std;

unsigned long long lcg( unsigned long long seed, 
                        unsigned long long a,
                        unsigned long long c, 
                        unsigned long long m, 
                        int n) {
    unsigned long long x = seed;

    for (int i = 0; i < n; i++) {
        x = (a * x + c) % m;
        //cout << "x" << i+1 << ": " << x << endl;
    }
    return x;
}

int main() {
    unsigned long long seed = 37; // seed
    unsigned long long a = 45; // multiplier
    unsigned long long c = 93; // increment
    unsigned long long m = 87; // modulus
    int values[] = {1000,
                    10000,
                    100000,
                    1000000,
                    10000000,
                    100000000
                }; //number of values

    for (int n : values) {
        auto start = chrono::high_resolution_clock::now();
        unsigned long long result = lcg(seed, a , c, m , n);

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

        cout << "n: " << n << "; ";
        cout << "Last generated value: " << result;
        cout << "; Time: " << duration.count() << " microseconds" << endl;
        }
    return 0;
}
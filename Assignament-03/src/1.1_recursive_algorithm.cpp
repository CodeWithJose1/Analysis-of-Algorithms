/*
        Analysis of Algorithms - Assignment 03

    Name: Jose Ignacio Cabascango Fernandez

    1.  Implement the Towers of Hanoi as a recursive algorithm and as an iterative algorithm. Compare
        the computational complexity of the algorithm with the mathematical analysis and with the
        empirical analysis techniques discussed during the lecture. Mathematical analysis was already
        done in class. Check for consistency between both methods.
*/

#include <iostream>
#include <chrono>
using namespace std;

// Recursive algorithm 
long long recursive = 0;
void recursive_hanoi(int n, char sourse, char aux, char destination) {
    if (n == 0) 
        return;
    
    recursive_hanoi(n - 1, sourse, destination, aux);
    recursive++;
    recursive_hanoi( n - 1, aux, sourse, destination);
}

// using command

int main() {

    int n = 30;

    auto start = chrono::high_resolution_clock::now();
    recursive_hanoi(n, 'A', 'B', 'C');

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    cout << "n = " << n << "; Total moves: " << recursive << "; Time: " << duration.count() << " microseconds" << endl;

    return 0;
}


// using chrono 
/*
int main() {

    int values[] = {10, 15, 20, 25, 30};

    for (int n : values) {

        recursive = 0;

        auto start = chrono::high_resolution_clock::now();
        recursive_hanoi(n, 'A', 'B', 'C');

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

        cout << "n = " << n << "; Total moves: " << recursive << "; Time: " << duration.count() << " microseconds" << endl;
    }
    return 0;
}
*/
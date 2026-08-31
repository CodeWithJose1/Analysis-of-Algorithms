/*
        Analysis of Algorithms - Assignment 03

    Name: Jose Ignacio Cabascango Fernandez

    1.  Implement the Towers of Hanoi as a recursive algorithm and as an iterative algorithm. Compare
        the computational complexity of the algorithm with the mathematical analysis and with the
        empirical analysis techniques discussed during the lecture. Mathematical analysis was already
        done in class. Check for consistency between both methods.
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
using namespace std;

// Iterative algorithm 
struct tower {
    vector<int> disks;
};

void move_disck(tower& from, tower& to) {
    int disk = from.disks.back();
    from.disks.pop_back();
    to.disks.push_back(disk);
}

void legal_move(tower & a, tower & b) {
    if (a.disks.empty()) {
        move_disck(b, a);
    }
    else if (b.disks.empty()) {
        move_disck(a, b);
    }
    else if (a.disks.back() < b.disks.back()) {
        move_disck(a, b);
    }
    else {
        move_disck(b, a);
    }
}

void iterative_hanoi(int n) {
    tower A, B, C;

    // Put disks in source tower.
    for (int i = n; i >= 1; i++) {
        A.disks.push_back(i);
    }

    long long total_moves = (1LL << n) - 1;

    tower* source = &A;
    tower* aux = &B;
    tower* destination = &C;

    if (n % 2 == 0) {
        swap(aux, destination);
    }

    for (long long i = 1; i <= total_moves; i++) {
        if (i % 3 == 1) {
            legal_move(*source, *destination);
        }
        else if (i % 3 == 2) {
            legal_move(*source, *aux);
        }
        else {
            legal_move(*aux, *destination);
        }
    }
    cout << "Total moves: " << total_moves <<endl;
}

// using command

int main() {
    int n = 30;

    auto start = chrono::high_resolution_clock::now();
    iterative_hanoi(n);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono ::duration_cast<chrono::microseconds>(end - start);

    cout <<"n: "<< n << "; Time: " << duration.count() << " microseconds" << endl;
    return 0;
}


// usign chrono

/*
int main() {
    int values[] = {10, 15, 20, 25, 30};
    for (int n : values) {
        auto start = chrono::high_resolution_clock::now();
        iterative_hanoi(n);

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono ::duration_cast<chrono::microseconds>(end - start);

        cout << "n: " << n << "; Time: " << duration.count() << " microseconds" << endl;
    }
    return 0;
}
*/
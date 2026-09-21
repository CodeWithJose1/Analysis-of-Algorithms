/*
        Analysis of Algorithms - Assignment 05

    Name: Jose Ignacio Cabascango Fernandez

    4. For a DAG with n vertices, what is the largest number of distinct solutions 
    the topological sorting problem can have? Show your solution.
*/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int permutation(int n) {
    if (n == 0 || n == 1) 
        return 1;

    else 
        return n * permutation(n - 1);
}

int main() {
    int n = 5;
    vector<int> vertices(n);

    for (int i = 0; i < n; i++) 
        vertices[i] = i;
    
    long long count = 0;

    do {
        count++;
        cout << "Ordering " << count << ": ";

        for ( int v: vertices)
            cout << v << " ";

        cout << endl;
    }
    while (next_permutation(vertices.begin(), vertices.end()));

    cout << "Total number of distinct solutions: " << count << endl;
    cout << "Permutation of " << n << " vertices: " << permutation(n) << endl;

    return 0;
}


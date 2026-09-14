/*
        Analysis of Algorithms - Assignment 04

    Name: Jose Ignacio Cabascango Fernandez

    2. Implement a Depth First Search algorithm to print the values of each node in a 
    graph. Decide which data structure to use: adjacency matrix or adjacency list.
*/

#include <iostream>
#include <vector>
#include <stack>
using namespace std;

void DFS(const vector<vector<int>>& graph, int start) {
    int n = graph.size();

    vector<bool> visited(n, false);
    stack<int> s;

    s.push(start);

    while (!s.empty()) {
        int current = s.top();
        s.pop();

        if (visited[current])
            continue;
        
        visited[current] = true;
        cout << current << " ";

        for (int i = graph[current].size() - 1; i >= 0; i --) {
            int neighbor = graph[current][i];
            
            if (!visited[neighbor]) {
                s.push(neighbor);
            }
        }
    }
}

int main() {
    vector<vector<int>> graph = {
        {1, 2},
        {0, 3, 4},
        {0, 5},
        {1},
        {1},
        {2},
    };

    cout << "DFS traversal: ";
    DFS(graph, 0);
    cout << endl;

    return 0;
}
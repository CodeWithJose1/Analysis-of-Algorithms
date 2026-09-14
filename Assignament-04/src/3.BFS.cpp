/*
        Analysis of Algorithms - Assignment 04

    Name: Jose Ignacio Cabascango Fernandez

    3. Implement a Breadth First algorithm to print the values of each node in a 
    graph. Decide which data structure to use: adjacency matrix or adjacency list.
*/

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

void BFS(const vector<vector<int>>& graph, int start) {
    int n = graph.size();

    vector<bool> visited(n, false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    while(!q.empty()) {
        int current = q.front();
        q.pop();

        cout << current << " ";

        for (int neightbor : graph[current]) {
            if (!visited[neightbor]) {
                visited[neightbor] = true;
                q.push(neightbor);
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

    cout << "BFS traversal: ";
    BFS(graph, 0);
    cout << endl;

    return 0;
}
/*
        Analysis of Algorithms - Assignment 09

    Name: Jose Ignacio Cabascango Fernandez

    4. Create a C++ program that implementes the Dijkstra’s Algorithms. Show and execution
    example.
*/
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <limits>
#include <algorithm>
using namespace std;

struct Edge {
    int to, weight;
};
using State = pair<long long, int>;

int main() {
    const int n = 12;
    const long long INF = numeric_limits<long long>::max() / 4;
    vector<string> name = {"a","b","c","d","e","f","g","h","i","j","k","l"};
    vector<vector<Edge>> adj(n);
    auto addEdge = [&](int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    };

    addEdge(0, 1, 3); // a-b
    addEdge(0, 2, 5); // a-c
    addEdge(0, 3, 4); // a-d
    addEdge(1, 4, 3); // b-e
    addEdge(1, 5, 6); // b-f
    addEdge(2, 3, 2); // c-d
    addEdge(2, 6, 4); // c-g
    addEdge(3, 4, 1); // d-e
    addEdge(3, 7, 5); // d-h
    addEdge(4, 5, 2); // e-f
    addEdge(4, 8, 4); // e-i
    addEdge(5, 9, 5); // f-j
    addEdge(6, 7, 3); // g-h
    addEdge(6, 10, 6); // g-k
    addEdge(7, 8, 6); // h-i
    addEdge(7, 10, 7); // h-k
    addEdge(8, 9, 3); // i-j
    addEdge(8, 11, 5); // i-l
    addEdge(9, 11, 9); // j-l
    addEdge(10, 11, 8); // k-l

    vector<long long> dist(n, INF);
    vector<int> parent(n, -1);
    priority_queue<State, vector<State>, greater<State>> pq;
    dist[0] = 0;
    pq.push({0, 0});

    while (!pq.empty()) {
        auto [du, u] = pq.top();
        pq.pop();
        if (du != dist[u]) continue;
        for (const Edge& e : adj[u]) {
            long long candidate = du + e.weight;
            if (candidate < dist[e.to]) {
                dist[e.to] = candidate;
                parent[e.to] = u;
                pq.push({candidate, e.to});
            }
        }
    }

    cout << " Vertex | Distance from a | Shortest path\n";
    cout << "--------------------------------------------\n";
    for (int v = 0; v < n; ++v) {
        cout << name[v] << "       | " << dist[v] << "                | ";
        vector<int> path;
        for (int x = v; x != -1; x = parent[x]) path.push_back(x);
        reverse(path.begin(), path.end());
        for (size_t i = 0; i < path.size(); ++i) {
            if (i) cout << " -> ";
            cout << name[path[i]];
        }
        cout << '\n';
    }
    return 0;
}
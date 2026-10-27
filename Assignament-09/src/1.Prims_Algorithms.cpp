/*
        Analysis of Algorithms - Assignment 09

    Name: Jose Ignacio Cabascango Fernandez

    2. Create a C++ program that implementes the Prim’s Algorithms. Show an execution example.
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
struct State {
    int key, vertex;
    bool operator>(const State& other) const {
        if (key != other.key) return key > other.key;
        return vertex > other.vertex;
    }
};

int main() {
    const int n = 5;
    const int INF = numeric_limits<int>::max() / 4;
    vector<string> name = {"a", "b", "c", "d", "e"};
    vector<vector<Edge>> adj(n);
    auto addEdge = [&](int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    };

    addEdge(0, 1, 5); // a-b
    addEdge(0, 2, 7); // a-c
    addEdge(0, 4, 2); // a-e
    addEdge(1, 3, 6); // b-d
    addEdge(1, 4, 3); // b-e
    addEdge(2, 3, 4); // c-d
    addEdge(2, 4, 4); // c-e
    addEdge(3, 4, 5); // d-e

    vector<int> key(n, INF), parent(n, -1);
    vector<bool> inTree(n, false);
    priority_queue<State, vector<State>, greater<State>> pq;

    for (int v = 0; v < n; ++v) {
        key[v] = (v == 0 ? 0 : INF);
        pq.push({key[v], v});
    }

    int totalWeight = 0;
    vector<pair<int,int>> mst;
    while (!pq.empty()) {
        State cur = pq.top();
        pq.pop();
        int u = cur.vertex;

        if (inTree[u] || cur.key != key[u]) continue;
        inTree[u] = true;

        if (parent[u] != -1) {
            mst.push_back({parent[u], u});
            totalWeight += key[u];
            cout << name[parent[u]] << " - " << name[u]
                << " (weight " << key[u] << ")\n";
        }

        for (const Edge& e : adj[u]) {
            int v = e.to;
            if (!inTree[v] && e.weight < key[v]) {
                key[v] = e.weight;
                parent[v] = u;
                pq.push({key[v], v});
            }
        }
    }

    cout << "Total weight of the MST: " << totalWeight << '\n';
    return 0;
}
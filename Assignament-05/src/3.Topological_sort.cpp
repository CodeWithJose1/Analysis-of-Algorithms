/*
        Analysis of Algorithms - Assignment 05

    Name: Jose Ignacio Cabascango Fernandez

    3. Prove that the topological sorting problem has a solution if and only if 
    it is a DAG. 
*/

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

bool topological_sort(const vector<vector<int>>& graph, vector<int>& order) {
    int n = graph.size();
    vector<int> in_degree(n, 0);

    for (int u = 0; u < n; u++) {
        for (int v : graph[u]) {
            in_degree[v]++;
        }
    }

    queue<int> q;

    for (int i = 0; i < n; i++) {
        if (in_degree[i] == 0)
            q.push(i);
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        order.push_back(u);

        for (int v : graph[u]) {
            in_degree[v]--;

            if (in_degree[v] == 0)
                q.push(v);
        }
    }
    return order.size() == n;
}

void test_graph(const vector<vector<int>>& graph) {
    vector <int> order;
    bool is_DAG = topological_sort(graph, order);
    cout << "Graph: ";

    if (is_DAG) {
        cout << "DAG\n";
        cout << "Topological order: ";

        for (int v : order) 
            cout << v << " ";
        
        cout << "\n";
        cout << "Topological sort exists.\n";
    }
    else {
        cout << "Constains cycle\n";
        cout << "Topo;logical sort does not exist.\n";
    }
    cout << endl;
}

int main() {
    //Graph 1: DAG
    vector <vector<int>> dag = {
        {1},
        {2},
        {3},
        {}
    };

    //Graph 2: Contains cycle
    vector <vector<int>> cylic = {
        {1},
        {2},
        {3},
        {0}
    };

    cout << "Test\n";
    test_graph(dag);

    cout << "Test\n";
    test_graph(cylic);

    return 0;
}
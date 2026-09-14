/*
        Analysis of Algorithms - Assignment 04

    Name: Jose Ignacio Cabascango Fernandez

    1. Solve the 0/1 Knapsack problem using exhaustive search. Generate the problem, 
    capacity, weights, and values randomly for a problem size n. You could also use a 
    benchmark dataset if you wish. Find which is the biggest problem size n that you 
    can solve in 30s of execution. You will have to record in a table the execution 
    time for each n = 3, 4, 5, 6, … 
*/

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
using namespace std;

struct object{
    int weight;
    int value;
};

struct result{
    int best_value;
    int best_weight;
    long long combinations;
};

result knapsack(const vector<object> items, int capacity) {
    int n = items.size();
    int best_weight = 0;
    int best_value = 0;

    long long total_combinations = 1LL << n;
    
    for (long long mask = 0; mask < total_combinations; mask++) {
        int current_weight = 0;
        int current_value = 0;

        for (int i = 0; i < n; i++) {
            if (mask & (1LL << i)) {
                current_weight += items[i].weight;
                current_value += items[i].value;
            }
        }

        if (current_weight <= capacity && current_value > best_value) {
            best_value = current_value;
            best_weight = current_weight;
        }
    }
    return {best_value, best_weight, total_combinations};
}

int main () {
    random_device rd;
    mt19937 generator(rd());

    uniform_int_distribution<int> weight_dist(1, 100);
    uniform_int_distribution<int> value_dist(1, 100);

    cout << fixed << setprecision(6);
    cout << "n\tCombinations\tTime (seconds)\n";

    for (int n = 3; n <= 30; n++) {
        vector<object> items;
        int total_weight = 0;

        for (int i = 0; i < n; i++) {
            int weight = weight_dist(generator);
            int value = value_dist(generator);

            items.push_back({weight, value});
            total_weight += weight;
        }

        int capacity = total_weight / 2;
        auto start = chrono::high_resolution_clock::now();

        result Result = knapsack(items, capacity);
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double> elapsed = end - start;

        cout << n << "\t" << Result.combinations << "\t\t" << elapsed.count() << endl;

        if (elapsed.count() >= 30.0) {
            cout << "\n30-seconds limit reached at n = " << n << endl;
            break;
        }
    }
    return 0;
}
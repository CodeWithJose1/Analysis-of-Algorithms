/*
        Analysis of Algorithms - Assignment 06

    Name: Jose Ignacio Cabascango Fernandez

    1. Implement the Merge Sort algorithm.
*/
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

struct item {
    int key;
    string label;
};

void merge(vector<item>& a, int left, int mid, int right) {
    vector<item> temp;
    int i = left;
    int j = mid + 1;

    while (i <= mid && j <= right) {
        if(a[i].key <= a[j].key) {
            temp.push_back(a[i++]);
        } else {
            temp.push_back(a[j++]);
        }
    }

    while (i <= mid) {
        temp.push_back(a[i++]);
    }

    while (j <= right) {
        temp.push_back(a[j++]);
    }

    for (int k = 0; k < (int)temp.size(); k++) {
        a[left + k] = temp[k];
    }
}

void merge_sort(vector<item>& a, int left, int right) {
    if (left >= right) {
        return;
    }
    int mid = left + (right - left) / 2;
    merge_sort(a, left, mid);
    merge_sort(a, mid + 1, right);
    merge(a, left, mid, right);
}

void print_items(const vector<item>& a) {
    for (const auto& item : a) {
        cout << "(" << item.key << ", " << item.label << ") ";
    }
    cout << endl;
}

int main() {
    vector<item> items = {
        {3, "A"}, {1, "B"}, {2, "C"}, 
        {3, "D"}, {1, "E"}, {2, "F"}
    };

    cout << "Original list: \n";
    print_items(items);
    merge_sort(items, 0, (int)items.size() - 1);
    cout << "Sorted list: \n";
    print_items(items);
    
    return 0;
}
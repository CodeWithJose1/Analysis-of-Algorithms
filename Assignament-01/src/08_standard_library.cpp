/*
        Analysis of Algorithms - Assignment 01

    Name: Jose Ignacio Cabascango Fernandez

    8. Explore which alternatives does C++ Standard Library provide to deal with stacks and queues. Provide examples.
*/

#include <iostream>
#include <stack>
#include <queue>
using namespace std;

// Usando <stack>
void prin_stack(stack<int> pila_s) {
    while (!pila_s.empty()) {
        cout << pila_s.top() << " ";
        pila_s.pop();
    }
    cout << endl;
}

// usando <queue>
void print_queue(queue<int> pila_q) {
    while (!pila_q.empty()) {
        cout << pila_q.front() << " ";
        pila_q.pop();
    }
    cout << endl;
}

// usando <priority_queue>
void print_pq(priority_queue<int> pila_pq) {
    while (!pila_pq.empty()) {
        cout << pila_pq.top() << " ";
        pila_pq.pop();
    }
    cout << endl;
}

int main() {
    // Usando <stack>
    stack<int> pila_s;
    cout << "Using <stack> LIFO: " << endl;
    
    pila_s.push(5);
    pila_s.push(6);
    pila_s.push(7);
    pila_s.push(8);

    cout << " Top: " << pila_s.top() << endl;
    cout << "pila_s: ";
    prin_stack(pila_s);

    pila_s.pop();

    cout << "Top after of pop: " << pila_s.top() << endl;
    cout << "pila_s: ";
    prin_stack(pila_s);

    cout << endl;

    // usando <queue>
    queue<int> pila_q;
    cout << "Using <queue> FIFO: " << endl;

    pila_q.push(5);
    pila_q.push(6);
    pila_q.push(7);
    pila_q.push(8);

    cout << "Front: " << pila_q.front() << endl;
    cout << "Pila: ";
    print_queue(pila_q);

    pila_q.pop();

    cout << "Front after of pop: " << pila_q.front() << endl;
    cout << "Pila: ";
    print_queue(pila_q);

    cout << endl;

    // usando <priority_queue>
    priority_queue<int> pila_pq;
    cout << "Using <priority_queue> highest priority first: " << endl;

    pila_pq.push(5);
    pila_pq.push(3);
    pila_pq.push(7);
    pila_pq.push(1);

    cout << pila_pq.top() << endl;
    cout << "Pila_pq: ";
    print_pq(pila_pq);
    
    return 0;
}
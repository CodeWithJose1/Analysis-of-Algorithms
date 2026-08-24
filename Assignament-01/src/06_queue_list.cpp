/*
        Analysis of Algorithms - Assignment 01

    Name: Jose Ignacio Cabascango Fernandez

    6. Find the way C++ uses to queue and dequeue elements to and from a list to behave like a queue. Create an example that showcases the idea.
*/

#include <iostream>
#include <list>
using namespace std;

int main() {
    list<int> queue; 

    //enqueue
    queue.push_back(5);
    queue.push_back(6);
    queue.push_back(7);
    queue.push_back(8);

    cout << "First element: " << queue.front() << endl;

    //print the queue before dequeue
    cout << " Queue: ";
    for (int i : queue){
        cout << i << " ";
    }
    cout << endl;

    //dequeue
    queue.pop_front();

    cout << "First element after the dequeue: " << queue.front() << endl;

    //print the queue after the dequeu
    cout << "Queue: ";
    for (int i : queue) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}
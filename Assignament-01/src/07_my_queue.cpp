/*
        Analysis of Algorithms - Assignment 01

    Name: Jose Ignacio Cabascango Fernandez

    7. Create a class that encapsulates the stack behavior describe in point 6 (Ex: MyQueue)
*/

#include <iostream>
#include <list>
using namespace std;

class MyQueue {
    private:
        list<int> element;

    public:
        void enqueue(int value) {
            element.push_back(value);
        }

        void dequeu() {
            element.pop_front();
        }

        int front() {
            if (!element.empty()) {
                return element.front();
            }
            return -1;
        }

        bool empty() {
            return element.empty();
        }

        int size() {
            return element.size();
        }

        void print() {
            for (int i : element) {
                cout << i << " ";
            }
            cout << endl;
        }

};

int main() {
    MyQueue queue;

    queue.enqueue(5);
    queue.enqueue(6);
    queue.enqueue(7);
    queue.enqueue(8);

    cout << "Front: " << queue.front() << endl;
    cout << "size: " << queue.size() << endl;
    cout << "Queue: ";
    queue.print();

    queue.dequeu();

    cout << "Front after of dequeue: " << queue.front() << endl;

    cout << "Front: " << queue.front() << endl;
    cout << "size: " << queue.size() << endl;
    cout << "Queue: ";
    queue.print();


    return 0;
}
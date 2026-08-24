/*
        Analysis of Algorithms - Assignment 01

    Name: Jose Ignacio Cabascango Fernandez

    4. Find the way C++ uses to push and pop elements to and from a list to behave like a stack. Create an example that showcases the idea.
*/

#include <iostream>
#include <list>
using namespace std;

int main() {
    list<int> stack;

    // push
    stack.push_front(5);
    stack.push_front(6);
    stack.push_front(7);
    stack.push_front(8);

    cout << "Insert element: " << stack.front() << endl;

    // print the stack before pop
    cout << "Stack: ";
    for (int i : stack) {
        cout << i << " ";
    }
    cout << endl;

    // pop
    stack.pop_front();
    cout << "After pop: " << stack.front() << endl;

    // print the stack after pop
    cout << "Stack: ";
    for (int i : stack) {
        cout << i << " ";
    }

    cout << endl;
    return 0;
}

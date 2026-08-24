/*
        Analysis of Algorithms - Assignment 01

    Name: Jose Ignacio Cabascango Fernandez

    5. Create a class that encapsulates the stack behavior describe in point 4 (Ex: MyStack)
*/

#include <iostream>
#include <list>
using namespace std;

class MyStack {
    private:
        list<int> elemnts;
    
    public:
        void push(int value) {
            elemnts.push_front(value);
        }

        void pop() {
            if (!elemnts.empty()) {
                elemnts.pop_front();
            }
        }

        int top() {
            if (!elemnts.empty()) {
                return elemnts.front();
            }
            return -1;
        }

        bool empty() {
            return elemnts.empty();
        }

        int size() {
            return elemnts.size();
        }

        void print()  const {
            for (int i : elemnts) {
                cout << i << " ";
            }
            cout << endl;
        }
};

int main() {
    MyStack stack;

    stack.push(5);
    stack.push(6);
    stack.push(7);
    stack.push(8);

    cout << "Top: " << stack.top() << endl;

    //print the stack before pop
    cout << "Stack: ";
    stack.print();


    stack.pop();
    cout << "Top after of pop: " << stack.top() << endl;

    // print the stack after pop
    cout << "Stack: ";
    stack.print();

    return 0;
}
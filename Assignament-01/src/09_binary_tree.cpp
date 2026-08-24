/*
        Analysis of Algorithms - Assignment 01

    Name: Jose Ignacio Cabascango Fernandez

    9. Create your own Binary Tree data structure and test the insertion of elements. Insertion will need its own algorithm, research on how to implement it.
*/

#include <iostream>
using namespace std;

class BinaryTree {
    private: 
        struct Node {
            int data;
            Node* left;
            Node* right;

            Node(int value) {
                data = value;
                left = nullptr;
                right = nullptr;
            }
        };
    
        Node* root;
        Node* insert(Node* node, int value) {
            // if we find an empty position
            if (node == nullptr) {
                return new Node(value);
            }

            // go ro the left
            if (value < node -> data) {
                node -> left = insert(node -> left, value);
            }

            // go to the right
            else {
                node -> right = insert(node -> right, value);
            }
            return node;
        }

        void inorder(Node* node) {
            if (node == nullptr) {
                return;
            }
            inorder(node -> left);
            cout << node -> data << " ";
            inorder(node -> right);
        }

    public:
        BinaryTree() {
            root = nullptr;
        }

        void insert(int value) {
            root = insert(root, value);
        }

        void print_inorde() {
            inorder(root);
            cout << endl;
        }
};

int main() {
    BinaryTree tree;

    tree.insert(13);
    tree.insert(6);
    tree.insert(12);
    tree.insert(8);
    tree.insert(14);
    tree.insert(10);
    tree.insert(11);
    tree.insert(7);
    tree.insert(5);
    tree.insert(9);

    cout << "Tree in order: ";
    tree.print_inorde();

    return 0;
}
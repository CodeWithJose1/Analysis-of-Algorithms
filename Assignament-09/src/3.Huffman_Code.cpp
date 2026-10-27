/*
        Analysis of Algorithms - Assignment 09

    Name: Jose Ignacio Cabascango Fernandez

    5. Huffman Code.
        5.1. Construct a Huffman code for the following data:
        5.2. Encode ABACABAD using the code of the question
        5.3. Decode 100010111001010 using the code of the question
*/
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

using namespace std;

struct Node {
    char symbol;
    double frequency;
    Node* left;
    Node* right;

    Node(char s, double f)
        : symbol(s), frequency(f), left(nullptr), right(nullptr) {}

    Node(Node* l, Node* r)
        : symbol('\0'),
        frequency(l->frequency + r->frequency),
        left(l), right(r) {}
};

struct Compare {
    bool operator()(const Node* a, const Node* b) const {
        if (a->frequency != b->frequency)
            return a->frequency > b->frequency;
        return a->symbol > b->symbol;
    }
};

void generate_codes(
    Node* root,
    const string& prefix,
    unordered_map<char, string>& codes
) {
    if (!root) return;

    if (!root->left && !root->right) {
        codes[root->symbol] = prefix.empty() ? "0" : prefix;
        return;
    }

    generate_codes(root->left, prefix + "0", codes);
    generate_codes(root->right, prefix + "1", codes);
}

string encode(
    const string& message,
    const unordered_map<char, string>& codes
) {
    string result;

    for (char c : message) {
        auto it = codes.find(c);

        if (it == codes.end()) {
            cerr << "Error: symbol not found: " << c << '\n';
            return "";
        }

        result += it->second;
    }

    return result;
}

string decode(const string& bits, Node* root) {
    string result;
    Node* current = root;

    for (char bit : bits) {
        if (bit != '0' && bit != '1') {
            cerr << "Error: the input must contain only 0s and 1s.\n";
            return "";
        }

        current = (bit == '0') ? current->left : current->right;

        if (!current) {
            cerr << "Error: invalid binary sequence.\n";
            return "";
        }

        if (!current->left && !current->right) {
            result += current->symbol;
            current = root;
        }
    }

    if (current != root) {
        cerr << "Error: incomplete sequence at the end.\n";
        return "";
    }

    return result;
}

void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    vector<pair<char, double>> frequencies = {
        {'A', 0.40},
        {'B', 0.10},
        {'C', 0.20},
        {'D', 0.15},
        {'_', 0.15}
    };

    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (const auto& item : frequencies) {
        pq.push(new Node(item.first, item.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top();
        pq.pop();

        Node* right = pq.top();
        pq.pop();

        pq.push(new Node(left, right));
    }

    Node* root = pq.top();

    unordered_map<char, string> codes;
    generate_codes(root, "", codes);

    cout << "Huffman Code\n";
    for (const auto& item : frequencies) {
        cout << (item.first == '_' ? "SPACE" : string(1, item.first))
            << " : " << codes[item.first] << '\n';
    }

    string message = "ABACABAD";
    string encoded = encode(message, codes);

    cout << "\nOriginal message:" << message << '\n';
    cout << "Encoded message: " << encoded << '\n';

    string bits = "100010111001010";
    string decoded = decode(bits, root);

    cout << "\nBits received:" << bits << '\n';
    cout << "Decoded message:";

    for (char c : decoded) {
        cout << (c == '_' ? ' ' : c);
    }
    cout << '\n';

    deleteTree(root);
    return 0;
}

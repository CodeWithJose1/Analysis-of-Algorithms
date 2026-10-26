/*
        Analysis of Algorithms - Assignment 08

    Name: Jose Ignacio Cabascango Fernandez

    4. Implement a hashing algorithm using C++ (no dictionaries or libraries allowed). Use closed
    hashing as the collision resolution method.
*/
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

class hash_table {
private:
    enum class State { EMPTY, OCCUPIED, DELETED };
    struct Slot {
        int key = 0;
        State state = State::EMPTY;
    };

    vector<Slot> table;
    size_t occupied = 0;

    size_t hash_function(int key) const {
        const long long m = static_cast<long long>(table.size());
        long long remainder = static_cast<long long>(key) % m;
        if (remainder < 0) remainder += m;
        return static_cast<size_t>(remainder);
    }

public:
    explicit hash_table(size_t capacity) : table(capacity) {
        if (capacity == 0) throw invalid_argument("Capacity must be positive.");
    }

    bool insert(int key) {
        if (occupied == table.size()) return false;

        const size_t start = hash_function(key);
        size_t firstDeleted = table.size();

        for (size_t step = 0; step < table.size(); ++step) {
            const size_t index = (start + step) % table.size();
            Slot& slot = table[index];

            if (slot.state == State::OCCUPIED && slot.key == key) {
                return false;
            }
            if (slot.state == State::DELETED && firstDeleted == table.size()) {
                firstDeleted = index;
            }
            if (slot.state == State::EMPTY) {
                const size_t target = (firstDeleted == table.size()) ? index : firstDeleted;
                table[target].key = key;
                table[target].state = State::OCCUPIED;
                ++occupied;
                return true;
            }
        }

        if (firstDeleted != table.size()) {
            table[firstDeleted].key = key;
            table[firstDeleted].state = State::OCCUPIED;
            ++occupied;
            return true;
        }
        return false;
    }

    bool search(int key) const {
        const size_t start = hash_function(key);
        for (size_t step = 0; step < table.size(); ++step) {
            const Slot& slot = table[(start + step) % table.size()];
            if (slot.state == State::EMPTY) return false;
            if (slot.state == State::OCCUPIED && slot.key == key) return true;
        }
        return false;
    }

    bool remove(int key) {
        const size_t start = hash_function(key);
        for (size_t step = 0; step < table.size(); ++step) {
            Slot& slot = table[(start + step) % table.size()];
            if (slot.state == State::EMPTY) return false;
            if (slot.state == State::OCCUPIED && slot.key == key) {
                slot.state = State::DELETED;
                --occupied;
                return true;
            }
        }
        return false;
    }

    void display() const {
        cout << "Index | Value\n";
        for (size_t i = 0; i < table.size(); ++i) {
            cout << i << "     | ";
            if (table[i].state == State::OCCUPIED) cout << table[i].key;
            else if (table[i].state == State::DELETED) cout << "DELETED";
            else cout << "EMPTY";
            cout << '\n';
        }
    }
};

int main() {
    hash_table table(11);
    int option;

    do {
        cout << "\nClosed Hashing (Linear Probing)\n"
            << "1. Insert\n2. Search\n3. Delete\n4. Display\n0. Exit\n"
            << "Option: ";
        if (!(cin >> option)) {
            cerr << "Invalid input.\n";
            return 1;
        }

        int key;
        switch (option) {
            case 1:
                cout << "Key to insert: ";
                if (!(cin >> key)) return 1;
                cout << (table.insert(key) ? "Inserted.\n" : "Not inserted (duplicate or table full).\n");
                break;
            case 2:
                cout << "Key to search: ";
                if (!(cin >> key)) return 1;
                cout << (table.search(key) ? "Key found.\n" : "Key not found.\n");
                break;
            case 3:
                cout << "Key to delete: ";
                if (!(cin >> key)) return 1;
                cout << (table.remove(key) ? "Deleted.\n" : "Key not found.\n");
                break;
            case 4:
                table.display();
                break;
            case 0:
                cout << "Exiting.\n";
                break;
            default:
                cout << "Choose a valid option.\n";
        }
    } while (option != 0);

    return 0;
}
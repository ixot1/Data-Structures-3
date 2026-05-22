#include <climits>
#include "open_addressing.h"

// Implementation of Open Addressing hash table
Open_Addressing::Open_Addressing(size_t initial_capacity)
    : capacity(initial_capacity), size(0) {
    table = new Open_Addressing_Node * [capacity];
    for (size_t i = 0; i < capacity; i++) {
        table[i] = nullptr;
    }
}

// Helper function to clean up memory
void Open_Addressing::destroy() {
    for (size_t i = 0; i < capacity; i++) {
        if (table[i] != nullptr) {
            delete table[i];
        }
    }
    delete[] table;
}

// Destructor to clean up memory
Open_Addressing::~Open_Addressing() {
    destroy();
}

// Simple hash function for strings
size_t Open_Addressing::hash(const std::string& key) const {
    size_t hash_value = 0;
    for (char c : key) {
        hash_value = (hash_value * 31 + c) % capacity;
    }
    return hash_value;
}

// Resize the hash table when load factor exceeds 0.7
void Open_Addressing::resize() {
    size_t old_capacity = capacity;
    Open_Addressing_Node** old_table = table;

    capacity *= 2;
    table = new Open_Addressing_Node* [capacity];
    for (size_t i = 0; i < capacity; i++) {
        table[i] = nullptr;
    }

    size = 0;
    for (size_t i = 0; i < old_capacity; i++) {
        if (old_table[i] != nullptr && !old_table[i]->isDeleted) {
            insert(old_table[i]->key, old_table[i]->value);
            delete old_table[i];
        }
    }

    delete[] old_table;
}

// Helper function to insert a node without resizing, returns pointer to the inserted node or nullptr if table is full
Open_Addressing_Node* Open_Addressing::insert_node(const std::string& key, int value) {
    size_t index = hash(key);
    size_t original = index;

    do {
        if (table[index] == nullptr || table[index]->isDeleted) {
            if (table[index] != nullptr) {
                delete table[index];
            }
            table[index] = new Open_Addressing_Node(key, value);
            size++;
            return table[index];
        }
        if (table[index]->key == key) {
            table[index]->value = value;
            return table[index];
        }
        index = (index + 1) % capacity;
    } while (index != original);

    return nullptr;
}

// Add key-value pair to the dictionary and if element with specified key already exists, then update the value
void Open_Addressing::insert(const std::string& key, int value) {
    if (static_cast<double>(size) / capacity >= 0.7) {
        resize();
    }

    Open_Addressing_Node* node = insert_node(key, value);
    if (!node) {
        resize();
        insert_node(key, value);
    }
}

// Remove element with specified key
void Open_Addressing::remove(const std::string& key) {
    size_t index = hash(key);
    size_t original = index;

    do {
        if (table[index] == nullptr) {
            return;
        }

        if (!table[index]->isDeleted &&
            table[index]->key == key) {

            table[index]->isDeleted = true;
            size--;
            return;
        }

        index = (index + 1) % capacity;

    } while (index != original);
}

// Return value of element with specified key
Open_Addressing_Node* Open_Addressing::find(const std::string& key) const {
    size_t index = hash(key);
    size_t original = index;

    do {
        if (table[index] == nullptr) {
            return nullptr;
        }
        if (!table[index]->isDeleted && table[index]->key == key) {
            return table[index];
        }
        index = (index + 1) % capacity;
    } while (index != original);

    return nullptr;
}
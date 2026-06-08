#include "open_addressing.h"
#include <cmath>

// Initializes an array of node pointers with nulls
Open_Addressing::Open_Addressing(size_t initial_capacity)
    : capacity(initial_capacity), size(0) {
    table = new Open_Addressing_Node * [capacity];
    for (size_t i = 0; i < capacity; i++) {
        table[i] = nullptr;
    }
}

// Iterates through the table to free memory for all allocated nodes
void Open_Addressing::destroy() {
    for (size_t i = 0; i < capacity; i++) {
        if (table[i] != nullptr) {
            delete table[i];
        }
    }
    delete[] table;
}

// Cleans up all allocated memory when the object is destroyed
Open_Addressing::~Open_Addressing() {
    destroy();
}

// Chooses the initial index using standard modulo arithmetic
size_t Open_Addressing::hash(int key) const {
    return static_cast<size_t>(std::abs(key)) % capacity;
}

// Doubles the table capacity and rehashes all active (non-deleted) elements
void Open_Addressing::resize() {
    size_t old_capacity = capacity;
    Open_Addressing_Node** old_table = table;

    capacity *= 2;
    table = new Open_Addressing_Node * [capacity];
    for (size_t i = 0; i < capacity; i++) {
        table[i] = nullptr;
    }

    size = 0;
    for (size_t i = 0; i < old_capacity; i++) {
        // Skip nulls and tombstones (isDeleted == true)
        if (old_table[i] != nullptr && !old_table[i]->isDeleted) {
            insert(old_table[i]->key, old_table[i]->value);
            delete old_table[i];
        }
    }
    delete[] old_table;
}

// Uses linear probing to find an empty slot or recycle a tombstone for insertion
Open_Addressing_Node* Open_Addressing::insert_node(int key, int value) {
    size_t index = hash(key);
    size_t original = index;

    do {
        // Insert if the slot is completely empty or marked as deleted
        if (table[index] == nullptr || table[index]->isDeleted) {
            if (table[index] != nullptr) {
                delete table[index];
            }
            table[index] = new Open_Addressing_Node(key, value);
            size++;
            return table[index];
        }
        // Update value if the key already exists
        if (table[index]->key == key) {
            table[index]->value = value;
            return table[index];
        }
        // Move to the next slot, wrapping around to the beginning if necessary
        index = (index + 1) % capacity;
    } while (index != original);

    return nullptr;
}

// Checks the load factor and resizes if it reaches the 0.7 threshold before inserting
void Open_Addressing::insert(int key, int value) {
    if (static_cast<double>(size) / capacity >= 0.7) {
        resize();
    }
    Open_Addressing_Node* node = insert_node(key, value);
    if (!node) {
        resize();
        insert_node(key, value);
    }
}

// Performs lazy deletion (tombstone method) by marking the node as deleted
void Open_Addressing::remove(int key) {
    size_t index = hash(key);
    size_t original = index;

    do {
        if (table[index] == nullptr) return;

        if (!table[index]->isDeleted && table[index]->key == key) {
            table[index]->isDeleted = true;
            size--;
            return;
        }
        index = (index + 1) % capacity;
    } while (index != original);
}

// Uses linear probing to search for a key, ignoring elements marked as deleted
Open_Addressing_Node* Open_Addressing::find(int key) const {
    size_t index = hash(key);
    size_t original = index;

    do {
        if (table[index] == nullptr) return nullptr;
        if (!table[index]->isDeleted && table[index]->key == key) {
            return table[index];
        }
        index = (index + 1) % capacity;
    } while (index != original);

    return nullptr;
}
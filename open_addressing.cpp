#include "open_addressing.h"
#include <cmath>

Open_Addressing::Open_Addressing(size_t initial_capacity)
    : capacity(initial_capacity), size(0) {
    table = new Open_Addressing_Node * [capacity];
    for (size_t i = 0; i < capacity; i++) {
        table[i] = nullptr;
    }
}

void Open_Addressing::destroy() {
    for (size_t i = 0; i < capacity; i++) {
        if (table[i] != nullptr) {
            delete table[i];
        }
    }
    delete[] table;
}

Open_Addressing::~Open_Addressing() {
    destroy();
}

size_t Open_Addressing::hash(int key) const {
    return static_cast<size_t>(std::abs(key)) % capacity;
}

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
        if (old_table[i] != nullptr && !old_table[i]->isDeleted) {
            insert(old_table[i]->key, old_table[i]->value);
            delete old_table[i];
        }
    }
    delete[] old_table;
}

Open_Addressing_Node* Open_Addressing::insert_node(int key, int value) {
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
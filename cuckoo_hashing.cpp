#include "cuckoo_hashing.h"
#include <cmath>

Cuckoo_Hashing::Cuckoo_Hashing(size_t initial_capacity)
    : capacity(initial_capacity), size(0) {
    table1 = new Cuckoo_Node * [capacity];
    table2 = new Cuckoo_Node * [capacity];
    for (size_t i = 0; i < capacity; i++) {
        table1[i] = nullptr;
        table2[i] = nullptr;
    }
}

Cuckoo_Hashing::~Cuckoo_Hashing() {
    destroy();
}

void Cuckoo_Hashing::destroy() {
    for (size_t i = 0; i < capacity; i++) {
        delete table1[i];
        delete table2[i];
    }
    delete[] table1;
    delete[] table2;
}

size_t Cuckoo_Hashing::hash1(int key) const {
    return static_cast<size_t>(std::abs(key)) % capacity;
}

size_t Cuckoo_Hashing::hash2(int key) const {
    size_t k = static_cast<size_t>(std::abs(key));
    return (k ^ (k * 2654435761u >> 16)) % capacity;
}

Cuckoo_Node* Cuckoo_Hashing::find(int key) const {
    size_t p1 = hash1(key);
    if (table1[p1] && table1[p1]->isOccupied && table1[p1]->key == key) {
        return table1[p1];
    }

    size_t p2 = hash2(key);
    if (table2[p2] && table2[p2]->isOccupied && table2[p2]->key == key) {
        return table2[p2];
    }
    return nullptr;
}

bool Cuckoo_Hashing::insert_with_replacement(int key, int value, bool useFirstHash, size_t iterations) {
    if (iterations >= max_loop_iterations_) return false;

    size_t pos = useFirstHash ? hash1(key) : hash2(key);
    Cuckoo_Node** table = useFirstHash ? table1 : table2;

    if (table[pos] == nullptr) {
        table[pos] = new Cuckoo_Node(key, value);
        size++;
        return true;
    }
    if (table[pos]->key == key) {
        table[pos]->value = value;
        return true;
    }

    int oldKey = table[pos]->key;
    int oldValue = table[pos]->value;
    table[pos]->key = key;
    table[pos]->value = value;

    return insert_with_replacement(oldKey, oldValue, !useFirstHash, iterations + 1);
}

void Cuckoo_Hashing::rehash() {
    size_t oldCapacity = capacity;
    Cuckoo_Node** old1 = table1;
    Cuckoo_Node** old2 = table2;

    capacity *= 2;
    table1 = new Cuckoo_Node * [capacity];
    table2 = new Cuckoo_Node * [capacity];

    for (size_t i = 0; i < capacity; i++) {
        table1[i] = nullptr;
        table2[i] = nullptr;
    }
    size = 0;

    for (size_t i = 0; i < oldCapacity; i++) {
        if (old1[i]) {
            insert(old1[i]->key, old1[i]->value);
            delete old1[i];
        }
        if (old2[i]) {
            insert(old2[i]->key, old2[i]->value);
            delete old2[i];
        }
    }
    delete[] old1;
    delete[] old2;
}

void Cuckoo_Hashing::insert(int key, int value) {
    Cuckoo_Node* existing = find(key);
    if (existing) {
        existing->value = value;
        return;
    }
    if (static_cast<double>(size) / capacity >= 0.5) {
        rehash();
    }
    size_t attempts = 0;
    while (attempts < max_rehash_attempts_) {
        if (insert_with_replacement(key, value, true)) return;
        rehash();
        attempts++;
    }
}

void Cuckoo_Hashing::remove(int key) {
    size_t p1 = hash1(key);
    if (table1[p1] && table1[p1]->isOccupied && table1[p1]->key == key) {
        delete table1[p1];
        table1[p1] = nullptr;
        size--;
        return;
    }
    size_t p2 = hash2(key);
    if (table2[p2] && table2[p2]->isOccupied && table2[p2]->key == key) {
        delete table2[p2];
        table2[p2] = nullptr;
        size--;
    }
}
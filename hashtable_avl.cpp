#include "hashtable_avl.h"
#include <cmath>

HashTable_AVL::HashTable_AVL(size_t cap) : capacity(cap) {
    buckets.resize(capacity);
}

size_t HashTable_AVL::hash(int key) const {
    return static_cast<size_t>(std::abs(key)) % capacity;
}

void HashTable_AVL::insert(int key, int value) {
    size_t index = hash(key);
    buckets[index].insert(key, value);
}

void HashTable_AVL::remove(int key) {
    size_t index = hash(key);
    buckets[index].remove(key);
}
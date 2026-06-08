#include "hashtable_avl.h"
#include <cmath>

// Initializes the vector of buckets (AVL Trees) with the specified capacity
HashTable_AVL::HashTable_AVL(size_t cap) : capacity(cap) {
    buckets.resize(capacity);
}

// Chooses the bucket index using standard modulo arithmetic
size_t HashTable_AVL::hash(int key) const {
    return static_cast<size_t>(std::abs(key)) % capacity;
}

// Routes the key-value pair to the appropriate AVL tree bucket
void HashTable_AVL::insert(int key, int value) {
    size_t index = hash(key);
    buckets[index].insert(key, value);
}

// Routes the deletion request to the corresponding AVL tree bucket
void HashTable_AVL::remove(int key) {
    size_t index = hash(key);
    buckets[index].remove(key);
}
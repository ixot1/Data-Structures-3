#pragma once
#include <vector>
#include "hash_tables.h"
#include "AVL_tree.h"

class HashTable_AVL : public Hash_Table {
private:
    std::vector<AVL_Tree> buckets;
    size_t capacity;  
    size_t hash(int key) const;

public:
    HashTable_AVL(size_t cap = 100000);

    void insert(int key, int value) override;
    void remove(int key) override;
};

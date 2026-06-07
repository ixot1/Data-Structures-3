#pragma once
#include "hash_tables.h"

struct Cuckoo_Node {
    bool isOccupied = false;
    int key;
    int value;

    Cuckoo_Node() : key(0), value(0), isOccupied(false) {}
    Cuckoo_Node(int k, int v) : key(k), value(v), isOccupied(true) {}
};

class Cuckoo_Hashing : public Hash_Table {
private:
    static const size_t max_rehash_attempts_ = 10;
    static const size_t max_loop_iterations_ = 100;

    Cuckoo_Node** table1;
    Cuckoo_Node** table2;
    size_t capacity;
    size_t size;

    Cuckoo_Node* find(int key) const;
    size_t hash1(int key) const;
    size_t hash2(int key) const;
    void rehash();
    void destroy();
    bool insert_with_replacement(int key, int value, bool useFirstHash, size_t iterations = 0);

public:
    Cuckoo_Hashing(size_t initial_capacity = 16);
    ~Cuckoo_Hashing();

    void insert(int key, int value) override;
    void remove(int key) override;
};
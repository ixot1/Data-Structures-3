#pragma once

#include "hash_tables.h"
#include <string>
#include <climits>

// Node structure for cuckoo hashing
struct Cuckoo_Node {
    bool isOccupied = false;

    std::string key;
    int value;

    Cuckoo_Node(): key(""), value(0), isOccupied(false) {}

    Cuckoo_Node(const std::string& k, int v): key(k), value(v),isOccupied(true) {}
};

// Cuckoo hashing implementation
class Cuckoo_Hashing : public Hash_Table {
private:

    static const size_t max_rehash_attempts_ = 10;

    static const size_t max_loop_iterations_ = 100;

    Cuckoo_Node** table1;
    Cuckoo_Node** table2;

    size_t capacity;
    size_t size;

    Cuckoo_Node* find(const std::string& key) const;

    size_t hash1(const std::string& key) const;

    size_t hash2(const std::string& key) const;

    void rehash();

    void destroy();

    bool insert_with_replacement(const std::string& key, int value, bool useFirstHash, size_t iterations = 0);

public:

    Cuckoo_Hashing(size_t initial_capacity = 16);

    ~Cuckoo_Hashing();

    // HashTable interface
    void insert(const std::string& key,int value) override;

    void remove(const std::string& key) override;

};
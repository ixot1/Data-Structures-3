#pragma once
#include "hash_tables.h"

// Node structure for open addressing hash table
struct Open_Addressing_Node {
    bool isDeleted = false;
    std::string key;
    int value;
    Open_Addressing_Node(const std::string& k, int v) : key(k), value(v) {}
};

// Open addressing hash table implementation
class Open_Addressing : public Hash_Table {
private:
    Open_Addressing_Node** table;
    size_t capacity;
    size_t size;

    void destroy();
    Open_Addressing_Node* find(const std::string& key) const;
    size_t hash(const std::string& key) const;
    Open_Addressing_Node* insert_node(const std::string& key, int value);
    void resize();

public:
    Open_Addressing(size_t initial_capacity = 16);
    ~Open_Addressing();

    void insert(const std::string& key, int value) override;
    void remove(const std::string& key) override;
};
#pragma once
#include "hash_tables.h"

struct Open_Addressing_Node {
    bool isDeleted = false;
    int key;
    int value;
    Open_Addressing_Node(int k, int v) : key(k), value(v) {}
};

class Open_Addressing : public Hash_Table {
private:
    Open_Addressing_Node** table;
    size_t capacity;
    size_t size;

    void destroy();
    Open_Addressing_Node* find(int key) const;
    size_t hash(int key) const;
    Open_Addressing_Node* insert_node(int key, int value);
    void resize();

public:
    Open_Addressing(size_t initial_capacity = 16);
    ~Open_Addressing();

    void insert(int key, int value) override;
    void remove(int key) override;
};
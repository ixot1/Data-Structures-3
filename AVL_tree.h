#pragma once
#include "hash_tables.h"

struct AVL_Node {
    int key;
    int value;
    int height;
    AVL_Node* left;
    AVL_Node* right;

    AVL_Node(int k, int v) : key(k), value(v), height(1), left(nullptr), right(nullptr) {}
};

class AVL_Tree : public Hash_Table {
private:
    AVL_Node* root;

    void destroy(AVL_Node* node);
    AVL_Node* insertNode(AVL_Node* node, int key, int value);
    AVL_Node* removeNode(AVL_Node* node, int key);
    AVL_Node* findNode(AVL_Node* node, int key) const;
    AVL_Node* minNode(AVL_Node* node);
    AVL_Node* rotateLeft(AVL_Node* node);
    AVL_Node* rotateRight(AVL_Node* node);

    int height(AVL_Node* node) const;
    int balanceFactor(AVL_Node* node) const;

public:
    AVL_Tree();
    ~AVL_Tree();

    void insert(int key, int value) override;
    void remove(int key) override;
    int get(int key) const;
};
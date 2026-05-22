#pragma once

#include "hash_tables.h"
#include <string>

struct AVL_Node {
    std::string key;
    int value;
    int height;

    AVL_Node* left;
    AVL_Node* right;

    AVL_Node(const std::string& k, int v): key(k), value(v), height(1), left(nullptr), right(nullptr) {}
};

class AVL_Tree : public Hash_Table {
private:
    AVL_Node* root;

    void destroy(AVL_Node* node);

    AVL_Node* insertNode(
        AVL_Node* node,
        const std::string& key,
        int value);

    AVL_Node* removeNode(
        AVL_Node* node,
        const std::string& key);

    AVL_Node* findNode(
        AVL_Node* node,
        const std::string& key) const;

    AVL_Node* minNode(AVL_Node* node);

    AVL_Node* rotateLeft(AVL_Node* node);
    AVL_Node* rotateRight(AVL_Node* node);

    int height(AVL_Node* node) const;
    int balanceFactor(AVL_Node* node) const;

    void printNode(AVL_Node* node) const;

public:
    AVL_Tree();
    ~AVL_Tree();

    void insert(
        const std::string& key,
        int value) override;

    void remove(
        const std::string& key) override;

    int get(
        const std::string& key) const;

    void print() const;
};
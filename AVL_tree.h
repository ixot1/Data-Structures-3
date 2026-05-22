#pragma once
#include "utility.h" // for int8_t
#include <string>
#include <iostream>

struct AVLNode {
    std::string key;
    int value;

    AVLNode(const std::string& k, int v)
        : key(k), value(v), left(nullptr), right(nullptr), height(1) {
    }
};

class AVLTree {
private:
    AVLNode* root = nullptr;

    void destroy(AVLNode* node);
    size_t get_height(AVLNode* node) const;
    AVLNode* rotate_right(AVLNode* node);
    AVLNode* rotate_left(AVLNode* node);
    int8_t balance_factor(AVLNode* node) const;
    AVLNode* balance_tree(AVLNode* node);
    AVLNode* insert_node(AVLNode* node, const std::string& key, int value);
    AVLNode* remove_node(AVLNode* node, const std::string& key, int& removed_value);
    AVLNode* find_node(AVLNode* node, const std::string& key) const;
    void print_in_order(AVLNode* node) const;

public:
    AVLTree();
    ~AVLTree();

    void insert(const std::string& key, int value);
    int remove(const std::string& key);
    AVLNode* find(const std::string& key) const;
    void print(void) const;
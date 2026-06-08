#include "AVL_tree.h"
#include <algorithm>

AVL_Tree::AVL_Tree() : root(nullptr) {}

// Cleans up all allocated memory when the tree object is destroyed
AVL_Tree::~AVL_Tree() {
    destroy(root);
}

// Recursively deletes all nodes
void AVL_Tree::destroy(AVL_Node* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

// Returns the height of a node 
int AVL_Tree::height(AVL_Node* node) const {
    return node ? node->height : 0;
}

// Calculates the balance factor to check if the tree needs rotation 
int AVL_Tree::balanceFactor(AVL_Node* node) const {
    return node ? height(node->left) - height(node->right) : 0;
}

// Performs a right rotation to balance the tree
AVL_Node* AVL_Tree::rotateRight(AVL_Node* y) {
    AVL_Node* x = y->left;
    AVL_Node* t = x->right;

    x->right = y;
    y->left = t;

    // Update heights after rotation
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;
    return x;
}

// Performs a left rotation to balance the tree
AVL_Node* AVL_Tree::rotateLeft(AVL_Node* x) {
    AVL_Node* y = x->right;
    AVL_Node* t = y->left;

    y->left = x;
    x->right = t;

    // Update heights after rotation
    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;
    return y;
}

// Recursively inserts a new node and rebalances the tree if necessary
AVL_Node* AVL_Tree::insertNode(AVL_Node* node, int key, int value) {
    if (!node) return new AVL_Node(key, value);

    if (key < node->key)
        node->left = insertNode(node->left, key, value);
    else if (key > node->key)
        node->right = insertNode(node->right, key, value);
    else {
        // Update the value if the key already exists
        node->value = value;
        return node;
    }

    // Update the height of the current node
    node->height = std::max(height(node->left), height(node->right)) + 1;

    // Get the balance factor to check if this node became unbalanced
    int balance = balanceFactor(node);

    // Perform necessary rotations
    // Left-Left case: requires a single right rotation
    if (balance > 1 && key < node->left->key)
        return rotateRight(node);

    // Right-Right case: requires a single left rotation
    if (balance < -1 && key > node->right->key)
        return rotateLeft(node);

    // Left-Right case: Left rotation on child, then right rotation on node
    if (balance > 1 && key > node->left->key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right-Left case: Right rotation on child, then left rotation on node
    if (balance < -1 && key < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// Finds the node with the smallest key, used during node deletion
AVL_Node* AVL_Tree::minNode(AVL_Node* node) {
    while (node->left) node = node->left;
    return node;
}

// Recursively deletes a node and rebalances the tree if necessary
AVL_Node* AVL_Tree::removeNode(AVL_Node* node, int key) {
    if (!node) return nullptr;

    if (key < node->key)
        node->left = removeNode(node->left, key);
    else if (key > node->key)
        node->right = removeNode(node->right, key);
    else {
        // Node with only one child or no child
        if (!node->left || !node->right) {
            AVL_Node* temp = node->left ? node->left : node->right;
            delete node;
            return temp;
        }
        // Node with two children: get the smallest in the right subtree
        AVL_Node* temp = minNode(node->right);
        node->key = temp->key;
        node->value = temp->value;
        node->right = removeNode(node->right, temp->key);
    }

    // If the tree had only one node, no need to balance
    if (!node) return node;

    // Update height of the current node
    node->height = std::max(height(node->left), height(node->right)) + 1;

    // 3. Get the balance factor
    int balance = balanceFactor(node);

    // 4. Rebalance the tree using rotations
    if (balance > 1 && balanceFactor(node->left) >= 0)
        return rotateRight(node);

    if (balance > 1 && balanceFactor(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && balanceFactor(node->right) <= 0)
        return rotateLeft(node);

    if (balance < -1 && balanceFactor(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

    // Recursively searches for a node by its key
AVL_Node* AVL_Tree::findNode(AVL_Node* node, int key) const {
    if (!node) return nullptr;
    if (key == node->key) return node;
    // Traverse left or right depending on the key value
    return key < node->key ? findNode(node->left, key) : findNode(node->right, key);
}

void AVL_Tree::insert(int key, int value) {
    root = insertNode(root, key, value);
}

void AVL_Tree::remove(int key) {
    root = removeNode(root, key);
}

int AVL_Tree::get(int key) const {
    AVL_Node* node = findNode(root, key);
    return node ? node->value : -1;
}
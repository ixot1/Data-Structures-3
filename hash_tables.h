#pragma once
#include <string>
#include <iostream>

class Hash_Table {
public:
    // Add key-value pair to the dictionary and if element with specified key already exists, then update the value
    virtual void insert(const std::string& key, int value) = 0;

    // Remove element with specified key
    virtual void remove(const std::string& key) = 0;

    virtual ~Hash_Table() {}
};
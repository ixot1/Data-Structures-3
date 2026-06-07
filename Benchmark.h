#pragma once
#include <vector>
#include <string>
#include <functional>

class Benchmark {
public:
    static const int REPETITIONS;
    static const int SEED;
    static const std::vector<int> SIZES;

    static void runAllTests();
    static void testInsertAVL();
    static void testRemoveAVL();
    static void testInsertOpenAddressing();
    static void testRemoveOpenAddressing();
    static void testInsertCuckoo();
    static void testRemoveCuckoo();
};
#include "Benchmark.h"
#include "hashtable_avl.h"
#include "open_addressing.h"
#include "cuckoo_hashing.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <string>

const int Benchmark::REPETITIONS = 10;
const int Benchmark::SEED = 12345;
const std::vector<int> Benchmark::SIZES = { 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000 };

// Helper class for generating uniformly distributed random integers
class RandomGenerator {
private:
    std::mt19937 generator;
    std::uniform_int_distribution<> distribution;
public:
    RandomGenerator(int seed, int min, int max) : generator(seed), distribution(min, max) {}
    int getNext() { return distribution(generator); }
};

// Generic function to benchmark insertion performance for a given structure
template <typename StructureType>
static void runBenchmark(const std::string& testName, const std::string& fileName, int numCopies,
    std::function<void(StructureType&, int, int)> operation)
{
    std::cout << "\n[ Starting benchmark: " << testName << " ]\n";
    std::ofstream file(fileName);
    if (!file.is_open()) return;

    // Write CSV headers for data output
    file << "Size_N;Avg_Time_ns\n";

    for (int n : Benchmark::SIZES) {
        double totalTimeNs = 0.0;
        for (int r = 0; r < Benchmark::REPETITIONS; ++r) {
            // Create independent copies of the structure to average out the results
            std::vector<StructureType> copies(numCopies);
            RandomGenerator prepDataRng(Benchmark::SEED + r, 1, 2000000);
            RandomGenerator prepValueRng(Benchmark::SEED + 999 + r, 1, 5000000);

            // Pre-fill the structures with initial background data
            for (int j = 0; j < n; ++j) {
                int key = prepDataRng.getNext();
                int val = prepValueRng.getNext();
                for (int i = 0; i < numCopies; ++i) {
                    copies[i].insert(key, val);
                }
            }

            RandomGenerator testDataRng(Benchmark::SEED + 777 + r, 2000001, 4000000);
            RandomGenerator testValueRng(Benchmark::SEED + 888 + r, 1, 5000000);

            // Start the high-resolution timer for the measured operations
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < numCopies; ++i) {
                operation(copies[i], testDataRng.getNext(), testValueRng.getNext());
            }
            auto end = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            totalTimeNs += (static_cast<double>(duration) / numCopies);
        }
        double avgTimeNs = totalTimeNs / Benchmark::REPETITIONS;
        file << n << ";" << avgTimeNs << "\n";
        std::cout << "   N = " << n << " \tfinished. Avg: " << avgTimeNs << " ns\n";
    }
    file.close();
}

// Generic function to benchmark deletion performance for a given structure
template <typename StructureType>
static void runRemoveBenchmark(const std::string& testName, const std::string& fileName, int numCopies,
    std::function<void(StructureType&, int)> operation)
{
    std::cout << "\n[ Starting benchmark: " << testName << " ]\n";
    std::ofstream file(fileName);
    if (!file.is_open()) return;
    file << "Size_N;Avg_Time_ns\n";

    for (int n : Benchmark::SIZES) {
        double totalTimeNs = 0.0;
        for (int r = 0; r < Benchmark::REPETITIONS; ++r) {
            std::vector<StructureType> copies(numCopies);
            std::vector<int> insertedKeysHistory;

            RandomGenerator prepDataRng(Benchmark::SEED + r, 1, 2000000);
            RandomGenerator prepValueRng(Benchmark::SEED + 999 + r, 1, 5000000);

            // Pre-fill the structures and track inserted keys to guarantee successful deletion
            for (int j = 0; j < n; ++j) {
                int key = prepDataRng.getNext();
                int val = prepValueRng.getNext();
                insertedKeysHistory.push_back(key);
                for (int i = 0; i < numCopies; ++i) {
                    copies[i].insert(key, val);
                }
            }

            RandomGenerator idxRng(Benchmark::SEED + r + 123, 0, n - 1);
            // Randomly select a known existing key to measure removal time
            int targetKey = insertedKeysHistory[idxRng.getNext() % n];

            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < numCopies; ++i) {
                operation(copies[i], targetKey);
            }
            auto end = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            totalTimeNs += (static_cast<double>(duration) / numCopies);
        }
        double avgTimeNs = totalTimeNs / Benchmark::REPETITIONS;
        file << n << ";" << avgTimeNs << "\n";
        std::cout << "   N = " << n << " \tfinished. Avg: " << avgTimeNs << " ns\n";
    }
    file.close();
}

// Executes all defined benchmarks
void Benchmark::runAllTests() {
    testInsertAVL();
    testRemoveAVL();
    testInsertOpenAddressing();
    testRemoveOpenAddressing();
    testInsertCuckoo();
    testRemoveCuckoo();
}

void Benchmark::testInsertAVL() {
    runBenchmark<HashTable_AVL>("hashTable_AVL::insert", "AVL_insert.csv", 50,
        [](HashTable_AVL& s, int key, int val) { s.insert(key, val); });
}

void Benchmark::testRemoveAVL() {
    runRemoveBenchmark<HashTable_AVL>("HashTable_AVL::remove", "AVL_remove.csv", 50,
        [](HashTable_AVL& s, int key) { s.remove(key); });
}

void Benchmark::testInsertOpenAddressing() {
    runBenchmark<Open_Addressing>("OpenAddr::insert", "OpenAddr_insert.csv", 50,
        [](Open_Addressing& s, int key, int val) { s.insert(key, val); });
}

void Benchmark::testRemoveOpenAddressing() {
    runRemoveBenchmark<Open_Addressing>("OpenAddr::remove", "OpenAddr_remove.csv", 50,
        [](Open_Addressing& s, int key) { s.remove(key); });
}

void Benchmark::testInsertCuckoo() {
    runBenchmark<Cuckoo_Hashing>("Cuckoo::insert", "Cuckoo_insert.csv", 50,
        [](Cuckoo_Hashing& s, int key, int val) { s.insert(key, val); });
}

void Benchmark::testRemoveCuckoo() {
    runRemoveBenchmark<Cuckoo_Hashing>("Cuckoo::remove", "Cuckoo_remove.csv", 50,
        [](Cuckoo_Hashing& s, int key) { s.remove(key); });
}
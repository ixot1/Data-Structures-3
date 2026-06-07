#include "BenchmarkMenu.h"
#include "Benchmark.h"
#include <iostream>

// Helper function for cleaning std::cin buffer
static void clearInput() {
    std::cin.clear();
    std::cin.ignore(100, '\n');
}

// Get and return a choice from the user 
static int getUserChoice() {
    int choice;
    while (!(std::cin >> choice)) {
        clearInput();
        std::cout << "Incorrect choice, please try again. \n";
    }
    return choice;
}

static void methodMenu(int tableType) {
    while (true) {
        std::cout << "\n--- Choose the method for benchmarking. ---\n";
        std::cout << "1. insert (add key-value pair / update existing)\n";
        std::cout << "2. remove (delete element with specified key)\n";
        std::cout << "3. RUN ALL TESTS FOR THIS STRUCTURE\n";
        std::cout << "0. GO BACK\n";

        int methodChoice = getUserChoice();

        if (methodChoice == 0) break;

        std::cout << "\n Executing the benchmark... \n";

        switch (tableType) {
        case 1: // AVL Tree
            switch (methodChoice) {
            case 1:
                Benchmark::testInsertAVL();
                break;
            case 2:
                Benchmark::testRemoveAVL();
                break;
            case 3:
                Benchmark::testInsertAVL();
                Benchmark::testRemoveAVL();
                break;
            default:
                std::cout << "Incorrect choice, please try again \n";
                break;
            }
            break;

        case 2: // Open Addressing
            switch (methodChoice) {
            case 1:
                Benchmark::testInsertOpenAddressing();
                break;
            case 2:
                Benchmark::testRemoveOpenAddressing();
                break;
            case 3:
                Benchmark::testInsertOpenAddressing();
                Benchmark::testRemoveOpenAddressing();
                break;
            default:
                std::cout << "Incorrect choice, please try again \n";
                break;
            }
            break;

        case 3: // Cuckoo Hashing
            switch (methodChoice) {
            case 1:
                Benchmark::testInsertCuckoo();
                break;
            case 2:
                Benchmark::testRemoveCuckoo();
                break;
            case 3:
                Benchmark::testInsertCuckoo();
                Benchmark::testRemoveCuckoo();
                break;
            default:
                std::cout << "Incorrect choice, please try again \n";
                break;
            }
            break;
        }
    }
}

void runMainMenu() {
    while (true) {
        std::cout << "\n====== Choose the Hash Table / Dictionary variant for benchmarking ======\n";
        std::cout << "1. AVL Tree \n";
        std::cout << "2. Open Addressing \n";
        std::cout << "3. Cuckoo Hashing\n";
        std::cout << "0. EXIT\n";

        int structureChoice = getUserChoice();

        if (structureChoice == 0) {
            std::cout << "Closing program...\n";
            break;
        }

        if (structureChoice >= 1 && structureChoice <= 3) {
            methodMenu(structureChoice);
        }
        else {
            std::cout << "Invalid choice. Try 1, 2, or 3.\n";
        }
    }
}
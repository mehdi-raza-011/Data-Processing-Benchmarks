// *********************************************************
// Program: heap_sort.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC2L
// Tutorial Class: TT5L
// Trimester: 2610
// Member_1: [253UC256MS] | ALIF | [ALIF.SYAUQI.MOHD@student.mmu.edu.my] | [01165721400]  (Group Leader)
// Member_2: [252UC2559F] | RAZA MEHDI | [raza.mehdi@student.mmu.edu.my] | [01120907511]
// Member_3: [251UC2507N] | AHMAD DANIAL HAKIMI BIN AHMAD FADILLI | [ahmad.danial.hakimi@student.mmu.edu.my] | [0166026788]
// Member_4: [252UC254RB] | NEIL HARI A L HARIKRISHNAN | [neil.hari.harikrishan@student.mmu.edu.my] | [0123041704]
// *********************************************************
// Task Distribution
// Member_3 (DANIAL): Heap Sort
// Compile: g++ -O2 -std=c++17 -o heap_sort heap_sort.cpp file_reader.cpp
// Run    : ./heap_sort dataset_1000.csv
// *********************************************************

#include "file_reader.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <string>

using namespace std;

// Keeps the tree matching Max-Heap properties
void maxHeapify(Record* records, long long n, long long i) {
    long long largest = i;       // Assume root is biggest
    long long left = 2 * i + 1;  // Left child formula
    long long right = 2 * i + 2; // Right child formula

    // Check if left child is bigger
    if (left < n && records[left].key > records[largest].key) {
        largest = left;
    }

    // Check if right child is bigger
    if (right < n && records[right].key > records[largest].key) {
        largest = right;
    }

    // If a child is bigger, swap and keep heapifying down
    if (largest != i) {
        Record temp = records[i];
        records[i] = records[largest];
        records[largest] = temp;

        maxHeapify(records, n, largest);
    }
}

// Main heap sort logic
void heapSort(Record* records, long long n) {
    // Phase 1: Build the Max-Heap structure
    for (long long i = n / 2 - 1; i >= 0; i--) {
        maxHeapify(records, n, i);
    }

    // Phase 2: Pop out max elements from top to bottom
    for (long long i = n - 1; i > 0; i--) {
        // Swap root (biggest) with last element
        Record temp = records[0];
        records[0] = records[i];
        records[i] = temp;

        // Fix heap for the remaining unsorted part
        maxHeapify(records, i, 0);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <csv_file>\n";
        return 1;
    }

    long long n = 0;
    Record* data = readCSV(argv[1], n);
    if (!data) return 1;

    cout << "Sorting " << n << " items...\n";

    // Track execution time only for the algorithm
    auto tStart = chrono::high_resolution_clock::now();

    heapSort(data, n);

    auto tEnd = chrono::high_resolution_clock::now();
    double elapsedSec = chrono::duration<double>(tEnd - tStart).count();

    cout << "Running time: " << elapsedSec << " seconds.\n";

// Build output filename from input filename
string inputStr = string(argv[1]);
string sizeText = inputStr;
size_t pos1 = inputStr.find("dataset_");
size_t pos2 = inputStr.find(".csv");
if (pos1 != string::npos && pos2 != string::npos)
    sizeText = inputStr.substr(pos1 + 8, pos2 - pos1 - 8);

string outName = "heap_sort_dataset_" + sizeText + ".csv";
ofstream outFile(outName);
if (!outFile.is_open()) {
    cerr << "Error: cannot create " << outName << "\n";
    freeRecords(data);
    return 1;
}

for (long long i = 0; i < n; ++i)
    outFile << data[i].key << "/" << data[i].str << "\n";

outFile << "\nRunning time: " << elapsedSec << " seconds\n";
outFile.close();

cout << "Sorted output saved to: " << outName << "\n";
    freeRecords(data);
    return 0;
}

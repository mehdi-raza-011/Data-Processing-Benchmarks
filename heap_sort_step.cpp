// *********************************************************
// Program: heap_sort_step.cpp
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
// Member_3 (DANIAL): heap sort step
// *********************************************************
// Compile: g++ -O2 -std=c++17 -o heap_sort_step heap_sort_step.cpp file_reader.cpp
// Run    : ./heap_sort_step
// *********************************************************

#include "file_reader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

// ============================================================
// TUTOR DEMO INPUTS � change startRow / endRow as instructed
// ============================================================
static const char *DATASET_FILE = "dataset_1000.csv";
static const int START_ROW = 1; // first row to include (1-based)
static const int END_ROW = 7;   // last  row to include (1-based)

// ------------------------------------------------------------
// Print one snapshot of the array in the required format:
//   [key/str, key/str, ...] label
// ------------------------------------------------------------
void printStep(ofstream &out, Record *arr, int size, const string &label)
{
    out << "[";
    for (int i = 0; i < size; i++)
    {
        out << arr[i].key << "/" << arr[i].str;
        if (i < size - 1)
            out << ", ";
    }
    out << "] " << label << "\n";
}

// ------------------------------------------------------------
// maxHeapify � same logic as heap_sort.cpp
// ------------------------------------------------------------
void maxHeapify(Record *arr, int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].key > arr[largest].key)
        largest = left;
    if (right < n && arr[right].key > arr[largest].key)
        largest = right;

    if (largest != i)
    {
        Record tmp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = tmp;
        maxHeapify(arr, n, largest);
    }
}

// ------------------------------------------------------------
// heapSortStep � builds max-heap then extracts one by one,
//                printing a snapshot after each extraction.
// ------------------------------------------------------------
void heapSortStep(Record *arr, int size, ofstream &out)
{
    // ---- Phase 1: build max-heap (silent, no step printed) ----
    for (int i = size / 2 - 1; i >= 0; i--)
        maxHeapify(arr, size, i);

    // Print the heap state after building � label: "initial"
    printStep(out, arr, size, "initial");

    // ---- Phase 2: extract max elements one by one ----
    for (int i = size - 1; i > 0; i--)
    {
        // Swap root (max) with last unsorted element
        Record tmp = arr[0];
        arr[0] = arr[i];
        arr[i] = tmp;

        // Re-heapify the reduced heap
        maxHeapify(arr, i, 0);

        // Print snapshot � label: "i = N"
        printStep(out, arr, size, "i = " + to_string(i));
    }
}

// ============================================================
// MAIN
// ============================================================
int main()
{
    // ---- Read only rows START_ROW..END_ROW from the CSV ----
    ifstream inFile(DATASET_FILE);
    if (!inFile)
    {
        cerr << "Error: cannot open " << DATASET_FILE << "\n";
        return 1;
    }

    vector<Record> temp;
    string line;
    int rowNum = 1;

    while (getline(inFile, line) && rowNum <= END_ROW)
    {
        if (rowNum >= START_ROW)
        {
            // strip carriage return (Windows files)
            if (!line.empty() && line.back() == '\r')
                line.pop_back();

            size_t comma = line.find(',');
            if (comma == string::npos)
            {
                rowNum++;
                continue;
            }

            Record r;
            r.key = stoll(line.substr(0, comma));
            string s = line.substr(comma + 1);
            // copy up to 5 chars into the fixed char[6] field
            strncpy(r.str, s.c_str(), 5);
            r.str[5] = '\0';
            temp.push_back(r);
        }
        rowNum++;
    }
    inFile.close();

    int size = (int)temp.size();
    if (size == 0)
    {
        cerr << "No data in rows " << START_ROW << " to " << END_ROW << "\n";
        return 1;
    }

    // Copy to plain array
    Record *arr = new Record[size];
    for (int i = 0; i < size; i++)
        arr[i] = temp[i];

    // Build output filename: dataset_1000_heap_sorted_step_1_7.txt
    string outName = "dataset_1000_heap_sorted_step_" + to_string(START_ROW) + "_" + to_string(END_ROW) + ".txt";

    ofstream out(outName);
    if (!out.is_open())
    {
        cerr << "Error: cannot create " << outName << "\n";
        delete[] arr;
        return 1;
    }

    out << "(using maxheap)\n";
    heapSortStep(arr, size, out);
    out.close();

    delete[] arr;
    cout << "Done. Output written to " << outName << "\n";
    return 0;
}

// *********************************************************
// Program: hash_table_search.cpp
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
// Member_4 (NEIL):Step 5 / Hash Table Search experiment program.
//          Reads dataset_n.csv, builds a manual hash table
//          using AVL-tree buckets, then times n searches for
//          best, average, and worst cases.
//
// Compile:
//   g++ -O2 -std=c++17 -o hash_table_search hash_table_search.cpp file_reader.cpp
//
// Run:
//   ./hash_table_search dataset_1000.csv
//   ./hash_table_search dataset_1000000.csv
// *********************************************************

#include "file_reader.h"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// ============================================================
// AVL NODE USED INSIDE EACH HASH TABLE BUCKET
// ============================================================
struct AvlNode {
    Record rec;
    AvlNode* left;
    AvlNode* right;
    int height;

    explicit AvlNode(const Record& r)
        : rec(r), left(nullptr), right(nullptr), height(1) {}
};

static int nodeHeight(AvlNode* node)
{
    return node ? node->height : 0;
}

static int maxInt(int a, int b)
{
    return (a > b) ? a : b;
}

static void updateHeight(AvlNode* node)
{
    if (node)
        node->height = 1 + maxInt(nodeHeight(node->left), nodeHeight(node->right));
}

static int balanceFactor(AvlNode* node)
{
    return node ? nodeHeight(node->left) - nodeHeight(node->right) : 0;
}

static AvlNode* rotateRight(AvlNode* y)
{
    AvlNode* x = y->left;
    AvlNode* t2 = x->right;

    x->right = y;
    y->left = t2;

    updateHeight(y);
    updateHeight(x);
    return x;
}

static AvlNode* rotateLeft(AvlNode* x)
{
    AvlNode* y = x->right;
    AvlNode* t2 = y->left;

    y->left = x;
    x->right = t2;

    updateHeight(x);
    updateHeight(y);
    return y;
}

static AvlNode* insertAVL(AvlNode* node, const Record& rec)
{
    if (!node)
        return new AvlNode(rec);

    if (rec.key < node->rec.key) {
        node->left = insertAVL(node->left, rec);
    } else if (rec.key > node->rec.key) {
        node->right = insertAVL(node->right, rec);
    } else {
        // Dataset keys should be unique. If a duplicate appears, keep latest string.
        node->rec = rec;
        return node;
    }

    updateHeight(node);
    int bf = balanceFactor(node);

    // Left Left
    if (bf > 1 && rec.key < node->left->rec.key)
        return rotateRight(node);

    // Right Right
    if (bf < -1 && rec.key > node->right->rec.key)
        return rotateLeft(node);

    // Left Right
    if (bf > 1 && rec.key > node->left->rec.key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left
    if (bf < -1 && rec.key < node->right->rec.key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

static void deleteAVL(AvlNode* node)
{
    if (!node) return;
    deleteAVL(node->left);
    deleteAVL(node->right);
    delete node;
}

// ============================================================
// MANUAL HASH TABLE WITH AVL-TREE BUCKETS
// ============================================================
class HashTable {
private:
    AvlNode** buckets;
    long long tableSize;

    long long hashKey(long long key) const
    {
        long long h = key % tableSize;
        if (h < 0) h += tableSize;
        return h;
    }

public:
    explicit HashTable(long long size)
        : buckets(nullptr), tableSize(size)
    {
        if (tableSize < 1) tableSize = 1;
        buckets = new AvlNode*[tableSize];
        for (long long i = 0; i < tableSize; ++i)
            buckets[i] = nullptr;
    }

    ~HashTable()
    {
        for (long long i = 0; i < tableSize; ++i)
            deleteAVL(buckets[i]);
        delete[] buckets;
    }

    void insert(const Record& rec)
    {
        long long idx = hashKey(rec.key);
        buckets[idx] = insertAVL(buckets[idx], rec);
    }

    bool search(long long target, Record& found, long long& comparisons) const
    {
        comparisons = 0;
        long long idx = hashKey(target);
        AvlNode* cur = buckets[idx];

        while (cur) {
            ++comparisons; // one integer-key comparison against this record
            if (target == cur->rec.key) {
                found = cur->rec;
                return true;
            } else if (target < cur->rec.key) {
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
        return false;
    }
};

// ============================================================
// SMALL UTILITY FUNCTIONS
// ============================================================
static bool isPrime(long long x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if (x % 2 == 0) return false;
    for (long long d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

static long long nextPrime(long long x)
{
    if (x <= 2) return 2;
    if (x % 2 == 0) ++x;
    while (!isPrime(x)) x += 2;
    return x;
}

static string datasetSizeFromFilename(const string& filename, long long n)
{
    // Expected format: dataset_1000.csv
    size_t pos = filename.find("dataset_");
    if (pos != string::npos) {
        pos += 8; // length of "dataset_"
        size_t end = filename.find(".csv", pos);
        if (end != string::npos && end > pos)
            return filename.substr(pos, end - pos);
    }
    return to_string(n);
}

static void printRecordToStream(ostream& os, const Record& r)
{
    os << r.key << "/" << r.str;
}

static void printResultBlock(ostream& os,
                             const char* label,
                             const Record& targetRecord,
                             long long searches,
                             long long comparisonsTotal,
                             double seconds,
                             bool repeatedTarget)
{
    os << label << " case target: ";
    printRecordToStream(os, targetRecord);
    os << "\n";

    os << label << " case searches: " << searches;
    if (repeatedTarget)
        os << " repeated searches of the same target";
    else
        os << " successful searches, one per dataset record";
    os << "\n";

    os << label << " case total comparisons: " << comparisonsTotal << "\n";
    os << label << " case average comparisons/search: "
       << fixed << setprecision(4)
       << (searches > 0 ? (double)comparisonsTotal / (double)searches : 0.0)
       << "\n";
    os << label << " case time: " << fixed << setprecision(9)
       << seconds << " seconds\n\n";
}

// ============================================================
// MAIN
// ============================================================
int main(int argc, char* argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <dataset_n.csv>\n"
             << "Example: " << argv[0] << " dataset_1000.csv\n";
        return 1;
    }

    const string inputFile = argv[1];

    // --------------------------------------------------------
    // Read CSV first. File I/O is intentionally outside timing.
    // --------------------------------------------------------
    long long n = 0;
    Record* data = readCSV(inputFile.c_str(), n);
    if (!data || n <= 0) {
        cerr << "Error: could not load dataset.\n";
        return 1;
    }

    long long tableSize = nextPrime((n / 2) + 1);

    HashTable table(tableSize);
    for (long long i = 0; i < n; ++i)
        table.insert(data[i]);

    // --------------------------------------------------------
    // Pre-scan to find best and worst successful targets by
    // number of integer comparisons in the AVL bucket.
    // --------------------------------------------------------
    Record dummy;
    long long comps = 0;
    long long minComps = 9223372036854775807LL;
    long long maxComps = -1;
    Record bestTarget = data[0];
    Record worstTarget = data[0];

    for (long long i = 0; i < n; ++i) {
        if (table.search(data[i].key, dummy, comps)) {
            if (comps < minComps) {
                minComps = comps;
                bestTarget = data[i];
            }
            if (comps > maxComps) {
                maxComps = comps;
                worstTarget = data[i];
            }
        }
    }

    // Guard prevents the compiler from removing search calls.
    volatile long long checksum = 0;

    // --------------------------------------------------------
    // BEST CASE: n searches for a target with the smallest path.
    // --------------------------------------------------------
    long long bestTotalComparisons = 0;
    auto bestStart = chrono::high_resolution_clock::now();
    for (long long i = 0; i < n; ++i) {
        table.search(bestTarget.key, dummy, comps);
        bestTotalComparisons += comps;
        checksum += dummy.key & 1LL;
    }
    auto bestEnd = chrono::high_resolution_clock::now();
    double bestSeconds = chrono::duration<double>(bestEnd - bestStart).count();

    // --------------------------------------------------------
    // AVERAGE CASE: n successful searches, one for each key.
    // --------------------------------------------------------
    long long avgTotalComparisons = 0;
    auto avgStart = chrono::high_resolution_clock::now();
    for (long long i = 0; i < n; ++i) {
        table.search(data[i].key, dummy, comps);
        avgTotalComparisons += comps;
        checksum += dummy.key & 1LL;
    }
    auto avgEnd = chrono::high_resolution_clock::now();
    double avgSeconds = chrono::duration<double>(avgEnd - avgStart).count();

    // --------------------------------------------------------
    // WORST CASE: n searches for a target with the longest path.
    // --------------------------------------------------------
    long long worstTotalComparisons = 0;
    auto worstStart = chrono::high_resolution_clock::now();
    for (long long i = 0; i < n; ++i) {
        table.search(worstTarget.key, dummy, comps);
        worstTotalComparisons += comps;
        checksum += dummy.key & 1LL;
    }
    auto worstEnd = chrono::high_resolution_clock::now();
    double worstSeconds = chrono::duration<double>(worstEnd - worstStart).count();

    string sizeText = datasetSizeFromFilename(inputFile, n);
    string outName = "hash_table_search_dataset_" + sizeText + ".txt";

    ofstream out(outName.c_str());
    if (!out.is_open()) {
        cerr << "Error: cannot create output file " << outName << "\n";
        freeRecords(data);
        return 1;
    }

    out << "Hash Table Search Results\n";
    out << "Input file: " << inputFile << "\n";
    out << "Dataset size: " << n << "\n";
    out << "Hash table bucket count: " << tableSize << "\n";
    out << "Collision method: AVL tree bucket per hash index\n";
    out << "Timed section excludes CSV file I/O and output printing.\n";
    out << "Each timing performs n searches where n is the dataset size.\n\n";

    printResultBlock(out, "Best", bestTarget, n, bestTotalComparisons, bestSeconds, true);
    printResultBlock(out, "Average", data[0], n, avgTotalComparisons, avgSeconds, false);
    printResultBlock(out, "Worst", worstTarget, n, worstTotalComparisons, worstSeconds, true);

    out << "Checksum guard: " << checksum << "\n";
    out.close();

    cout << "Hash Table Search Results\n";
    cout << "Input file: " << inputFile << "\n";
    cout << "Dataset size: " << n << "\n";
    cout << "Hash table bucket count: " << tableSize << "\n";
    cout << "Output file: " << outName << "\n\n";

    printResultBlock(cout, "Best", bestTarget, n, bestTotalComparisons, bestSeconds, true);
    printResultBlock(cout, "Average", data[0], n, avgTotalComparisons, avgSeconds, false);
    printResultBlock(cout, "Worst", worstTarget, n, worstTotalComparisons, worstSeconds, true);

    freeRecords(data);
    return 0;
}

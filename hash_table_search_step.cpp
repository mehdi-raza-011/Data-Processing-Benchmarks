// *********************************************************
// Program: hash_table_search_step.cpp
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
// Member_4 (NEIL): Step-output version for Hash Table Search.
//          It shows the hash index and the exact search path
//          for one found target and one not-found target.
//
// Compile:
//   g++ -O2 -std=c++17 -o hash_table_search_step hash_table_search_step.cpp file_reader.cpp
//
// Run, default values below:
//   ./hash_table_search_step
//
// Run with custom demo values:
//   ./hash_table_search_step dataset_1000.csv 5807564266 123456789
// *********************************************************

#include "file_reader.h"

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// ============================================================
// TUTOR DEMO INPUTS
// ============================================================
static const char* DEFAULT_DATASET_FILE = "dataset_1000.csv";
static const long long DEFAULT_FOUND_TARGET = 5807564266LL; // first key in uploaded dataset_1000.csv
static const long long DEFAULT_NOT_FOUND_TARGET = 123456789LL;

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
        node->rec = rec;
        return node;
    }

    updateHeight(node);
    int bf = balanceFactor(node);

    if (bf > 1 && rec.key < node->left->rec.key)
        return rotateRight(node);

    if (bf < -1 && rec.key > node->right->rec.key)
        return rotateLeft(node);

    if (bf > 1 && rec.key > node->left->rec.key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

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

    long long bucketIndex(long long key) const
    {
        return hashKey(key);
    }

    const char* bucketType(long long target) const
    {
        long long idx = hashKey(target);
        AvlNode* root = buckets[idx];
        if (!root)
            return "empty bucket";
        if (!root->left && !root->right)
            return "direct";
        return "tree";
    }

    bool searchAndWritePath(long long target, ostream& out) const
    {
        long long idx = hashKey(target);
        AvlNode* cur = buckets[idx];

        out << "Search target: " << target << "\n";
        out << "Hash index: " << idx << "\n";
        out << "Search path type: " << bucketType(target) << "\n";
        out << "Path:\n";

        while (cur) {
            if (target == cur->rec.key) {
                out << target << " = " << cur->rec.key << "/" << cur->rec.str << "\n";
                out << "Result: FOUND\n";
                return true;
            }

            out << cur->rec.key << " != " << target << "\n";
            if (target < cur->rec.key)
                cur = cur->left;
            else
                cur = cur->right;
        }

        out << "-1 != " << target << "\n";
        out << "Result: NOT FOUND\n";
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
    size_t pos = filename.find("dataset_");
    if (pos != string::npos) {
        pos += 8;
        size_t end = filename.find(".csv", pos);
        if (end != string::npos && end > pos)
            return filename.substr(pos, end - pos);
    }
    return to_string(n);
}

static string makeStepFilename(const string& sizeText, long long target)
{
    return "dataset_" + sizeText + "_hash_table_search_step_" +
           to_string(target) + ".txt";
}

static void writeOneStepFile(const HashTable& table,
                             const string& inputFile,
                             const string& sizeText,
                             long long n,
                             long long target)
{
    string outName = makeStepFilename(sizeText, target);
    ofstream out(outName.c_str());
    if (!out.is_open()) {
        cerr << "Error: cannot create " << outName << "\n";
        return;
    }

    out << "Hash Table Search Step Output\n";
    out << "Input file: " << inputFile << "\n";
    out << "Dataset size: " << n << "\n";
    out << "Collision method: AVL tree bucket per hash index\n\n";
    table.searchAndWritePath(target, out);
    out.close();

    cout << "Created: " << outName << "\n";
}

// ============================================================
// MAIN
// ============================================================
int main(int argc, char* argv[])
{
    string inputFile = DEFAULT_DATASET_FILE;
    long long foundTarget = DEFAULT_FOUND_TARGET;
    long long notFoundTarget = DEFAULT_NOT_FOUND_TARGET;

    if (argc == 4) {
        inputFile = argv[1];
        foundTarget = atoll(argv[2]);
        notFoundTarget = atoll(argv[3]);
    } else if (argc != 1) {
        cerr << "Usage: " << argv[0] << "\n"
             << "   or: " << argv[0]
             << " <dataset_n.csv> <found_target> <not_found_target>\n";
        return 1;
    }

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

    string sizeText = datasetSizeFromFilename(inputFile, n);

    cout << "Hash table search step demo\n";
    cout << "Input file: " << inputFile << "\n";
    cout << "Dataset size: " << n << "\n";
    cout << "Hash table bucket count: " << tableSize << "\n";

    writeOneStepFile(table, inputFile, sizeText, n, foundTarget);
    writeOneStepFile(table, inputFile, sizeText, n, notFoundTarget);

    freeRecords(data);
    return 0;
}

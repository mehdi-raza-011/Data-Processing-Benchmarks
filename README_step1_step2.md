# CCP6214 – Step 1 & 2 Deliverables
## Dataset Generator + Shared File Reader

---

## Files in this package

| File | Step | Purpose |
|------|------|---------|
| `dataset_generator.cpp` | 1 | Generate `dataset_n.csv` |
| `file_reader.h` | 2 | Shared header – include in every algorithm file |
| `file_reader.cpp` | 2 | Shared implementation – compile alongside every algorithm |
| `dataset_1000.csv` | 1 | Pre-generated sample (1,000 records) |

---

## Step 1 – dataset_generator

### ⚠️ Before compiling: set ALIF's seed

Open `dataset_generator.cpp` and find line ~50:

```cpp
static const unsigned long long LEADER_SEED = 2531325639ULL; // <-- REPLACE THIS
```

Convert ALIF's student ID using the mapping table:
```
A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6
```
Example: `253UC256MS` → `2531325639`

### Compile

```bash
g++ -O2 -std=c++17 -o dataset_generator dataset_generator.cpp
```

### Run

```bash
./dataset_generator <n>

# Examples:
./dataset_generator 1000          # → dataset_1000.csv
./dataset_generator 1000000       # → dataset_1000000.csv
./dataset_generator 10000000      # → dataset_10000000.csv
```

### Dataset format

Each line: `<10-digit integer>,<5-letter string>`
```
9360601514,ivkib
9823888853,cgihs
1379986800,lmjql
```
- Integers: unique, random, range [1,000,000,000 – 9,999,999,999]
- Strings: random 5 lowercase letters
- Order: randomised (not sorted)

### Recommended input sizes for experiments (≥ 10 sizes)

| n | Approx. file size | Generation time |
|---|-------------------|-----------------|
| 1,000 | 17 KB | < 1 s |
| 10,000 | 167 KB | < 1 s |
| 100,000 | 1.7 MB | < 1 s |
| 500,000 | 8.5 MB | < 1 s |
| 1,000,000 | 17 MB | ~0.5 s |
| 5,000,000 | 85 MB | ~2.5 s |
| 10,000,000 | 170 MB | ~5 s |
| 20,000,000 | 340 MB | ~10 s |
| 50,000,000 | 850 MB | ~30 s |
| 100,000,000 | 1.7 GB | ~60 s |

> **Memory note (unordered_set):**
> ~40 bytes per entry → 10 M ≈ 400 MB | 50 M ≈ 2 GB | 100 M ≈ 4 GB.
> 100 M requires ~16 GB RAM. Use 50 M or less on 8 GB machines.

---

## Step 2 – file_reader (shared module)

### How team members use it

Every algorithm program should:
1. Copy `file_reader.h` and `file_reader.cpp` into the same folder.
2. Add `#include "file_reader.h"` at the top of their `.cpp` file.
3. Compile together:

```bash
# Radix sort example
g++ -O2 -std=c++17 -o radix_sort radix_sort.cpp file_reader.cpp

# Heap sort example
g++ -O2 -std=c++17 -o heap_sort heap_sort.cpp file_reader.cpp

# Hash table search example
g++ -O2 -std=c++17 -o hash_table_search hash_table_search.cpp file_reader.cpp
```

### API

```cpp
#include "file_reader.h"

// ---- Record struct ----
struct Record {
    long long key;  // 10-digit integer
    char      str[6]; // 5-letter string + '\0'
};

// ---- Load dataset ----
long long n = 0;
Record* data = readCSV("dataset_1000000.csv", n);
if (!data) { /* handle error */ return 1; }
// data[0] ... data[n-1] are now ready

// ---- Use data ----
// Sort data[] by key using your algorithm here ...

// ---- Clean up ----
freeRecords(data);

// ---- Print one record (integer/string format) ----
printRecord(data[i]);  // prints e.g. "1000000038/uoren"
```

### Time complexity of readCSV()

| Operation | Complexity |
|-----------|------------|
| Count lines (pass 1) | O(file bytes) |
| Parse records (pass 2) | O(n) |
| **Total** | **O(n)** |
| Space | O(n) |

I/O time is **excluded from algorithm timing** as required by the assignment.

---

## Test the file_reader standalone

```bash
g++ -O2 -std=c++17 -DTEST_READER -o test_reader file_reader.cpp
./test_reader dataset_1000.csv
```

---

## Quick start (all-in-one commands)

```bash
# 1. Compile everything
g++ -O2 -std=c++17 -o dataset_generator dataset_generator.cpp
g++ -O2 -std=c++17 -DTEST_READER -o test_reader file_reader.cpp

# 2. Generate sample datasets
./dataset_generator 1000
./dataset_generator 1000000
./dataset_generator 10000000

# 3. Verify file_reader
./test_reader dataset_1000.csv
```

---

## 🛠️ Step 4 – heap_sort
 
**Deliverables:** `heap_sort.cpp`, `heap_sort_step.cpp`

### Sequence of Workflow

1. **Data Loading:** Passes the dataset file through the terminal. The program uses the shared `readCSV()` function to safely load records into memory, completely skipping file I/O time during sorting benchmarks.
2. **Phase 1 (Build Max-Heap):** Starts at the last non-leaf node (`n / 2 - 1`) and loops backward to the root node, calling `maxHeapify` to arrange all items into a proper binary max-heap tree structure.
3. **Phase 2 (Extract & Sort):** Swaps the maximum root value at index `0` with the element at the end of the active array boundary. It shrinks the boundary size by 1 and runs `maxHeapify` on the root again, repeating until the elements are fully sorted in ascending order.
4. **Output Tracking:** * `heap_sort.cpp` wraps the main sort logic in high-resolution `<chrono>` clocks to track execution speeds in seconds and prints a terminal preview.
   * `heap_sort_step.cpp` logs structural milestones (after building the heap, individual extraction iterations, and final states) as space-separated blocks inside a custom generated output `.txt` file.
5. **Memory Clean up:** Invokes `freeRecords()` to wipe the allocated dynamic array block and safely shut down without causing memory leaks.

### Compile and Run

To compile and execute the Step 4 sorting programs, use these commands:

**Benchmark Timing Program:**
```bash
g++ -O2 -std=c++17 -o heap_sort heap_sort.cpp file_reader.cpp
./heap_sort dataset_1000.csv
```

---

## Step 5 - Hash Table Search

### Purpose

This step implements Hash Table Search using the integer field as the search key.

Two programs are provided:

| File | Purpose |
|---|---|
| `hash_table_search.cpp` | Runs the hash table search experiment and records best, average, and worst case running times |
| `hash_table_search_step.cpp` | Produces step-by-step search path output for a found target and a not-found target |

The implementation uses a manual hash table. Each hash bucket stores records using an AVL tree to handle collisions. Searching is performed using the 10-digit integer key, not the 5-letter string.

---

### Required files

Place these files in the same folder:

```text
hash_table_search.cpp
hash_table_search_step.cpp
file_reader.h
file_reader.cpp
dataset_n.csv

Example dataset:

dataset_1000.csv


# Compile
Compile the experiment program
g++ -O2 -std=c++17 -o hash_table_search hash_table_search.cpp file_reader.cpp
Compile the step-output program
g++ -O2 -std=c++17 -o hash_table_search_step hash_table_search_step.cpp file_reader.cpp

On Windows, the same commands can be used if g++ is installed through MinGW/MSYS2.

# Run hash_table_search

This program loads the dataset, builds the hash table, and measures running time for best, average, and worst case searches.

./hash_table_search dataset_1000.csv

Windows CMD:

hash_table_search dataset_1000.csv

For other input sizes, replace the dataset filename:

./hash_table_search dataset_100000.csv
./hash_table_search dataset_1000000.csv

Output from hash_table_search

The program creates an output file using this format:

hash_table_search_dataset_n.txt

Example:

hash_table_search_dataset_1000.txt

The output file contains:

Best case time: ...
Average case time: ...
Worst case time: ...

It also prints the same timing results in the command prompt window, so screenshots can be taken for the report.

The timed section excludes CSV file reading and output printing. Only the actual search operations are timed.

Run hash_table_search_step

This program creates search-path output files for one found target and one not-found target.

Default run:

./hash_table_search_step

Windows CMD:

hash_table_search_step

The default version uses:

dataset_1000.csv
Found target: 5807564266
Not-found target: 123456789

For a custom dataset and custom targets:

./hash_table_search_step dataset_1000.csv 5807564266 123456789

Windows CMD:

hash_table_search_step dataset_1000.csv 5807564266 123456789

Important: the found target must exist in the dataset. For example, in the provided dataset_1000.csv, the first row is:

5807564266,yyktq

So 5807564266 can be used as a found target.

The not-found target should be a number that does not exist in the dataset. 123456789 is suitable because it is outside the required 10-digit dataset range.


Output from hash_table_search_step

The step program creates two output files:

dataset_1000_hash_table_search_step_5807564266.txt
dataset_1000_hash_table_search_step_123456789.txt

The found-target file shows the search path until the target is found, for example:

5807564266 = 5807564266/yyktq
Result: FOUND

The not-found-target file shows the comparisons made until the search fails, ending with:

-1 != 123456789
Result: NOT FOUND

Experiment instructions

For the final experiment phase, run hash_table_search on at least 10 different dataset sizes.

Example input sizes:

1,000
10,000
100,000
500,000
1,000,000
5,000,000
10,000,000
20,000,000
50,000,000
100,000,000

For each input size:

Run the program.
Save the generated .txt output file.
Take a screenshot of the command prompt output.
Record the best, average, and worst case running times in the report.

Example:

./hash_table_search dataset_1000.csv
./hash_table_search dataset_10000.csv
./hash_table_search dataset_100000.csv


**“For demo, update the found target if using a newly generated dataset.”** 


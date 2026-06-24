
This repository automates the generation, high-speed organization, and near-instant retrieval of massive datasets. It provides a standardized framework to benchmark how different computational structures scale from small samples up to 50,000,000 records.

This guide provides step-by-step instructions on how to compile and execute each module using the Windows Command Prompt and the MinGW compiler.

Environment & Prerequisites
Operating System: Windows 10 / 11 ,Linux or others may be used but commands will differ.

Compiler: MinGW (GCC/G++) via Code::Blocks

Language Standard: C++17

⚠️ Important: All .cpp files, header files (e.g., file_reader.h), and generated .csv datasets must reside in the exact same directory for the relative paths to resolve correctly.

Step 0: Set Compiler Path
Before compiling any modules, open cmd in the project directory and configure your compiler path:
set "PATH=C:\Program Files\CodeBlocks\MinGW\bin;%PATH%"

1) Data Synthesis Engine
Synthesizes structured relational datasets (numeric IDs and alphanumeric payloads) to ensure all algorithms are benchmarked against identical baseline data limits.

Compile:
g++ -O2 -std=c++17 -o dataset_generator dataset_generator.cpp

Execute (Batch Generate All Sizes):
for %N in (1000 5000 10000 150000 500000 1000000 7500000 10000000 15000000 50000000) do dataset_generator %N

Note: This will generate 10 .csv files (e.g., dataset_1000.csv) directly into your project folder,You may mess with the datasets.

2) Positional Key Ordering Engine
Reads the generated datasets, sorts them mathematically from the rightmost character utilizing a stable counting sort, and outputs the execution time alongside the sorted files.

Compile:
g++ -O3 radix_sort.cpp -o radix_sort

Execute (Batch Sort):
for %N in (1000 5000 10000 150000 500000 1000000 7500000 10000000 15000000 50000000) do radix_sort.exe dataset_%N.csv

Note: The program will create new files prefixed with radix_sorted_ containing the ordered data and execution metrics.

3) Structural In-Place Processing Engine
Utilizes an in-place binary Max-Heap to sort the datasets. This module relies on the shared file_reader.cpp utility to handle file I/O operations.

Compile:
g++ -O3 heap_sort.cpp file_reader.cpp -o heap_sort

Execute (Batch Sort):
for %N in (1000 5000 10000 150000 500000 1000000 7500000 10000000 15000000 50000000) do heap_sort.exe dataset_%N.csv

4) Deterministic Lookup Engine
Tests average-case retrieval efficiency by mapping the generated datasets into a hash structure and measuring the exact lookup time. It relies on the shared file_reader.cpp utility.

Compile Main Engine:
g++ -O2 -std=c++17 -o hash_table_search hash_table_search.cpp file_reader.cpp

Compile Diagnostic Stepper (Optional):
g++ -O2 -std=c++17 -o hash_table_search_step hash_table_search_step.cpp file_reader.cpp

Execute (Batch Search Benchmark):
for %N in (1000 5000 10000 150000 500000 1000000 7500000 10000000 15000000 50000000) do hash_table_search dataset_%N.csv

 Troubleshooting & Common Errors
Error: 'g++' is not recognized
Cause & Solution: The MinGW compiler path is not set in your terminal. Run the Step 0 command to map the directory.

Error: fatal error: file_reader.h: No such file
Cause & Solution: Ensure file_reader.h is present in the root folder alongside your source files.

Error: undefined reference to readCSV
Cause & Solution: Missing file reader source links. Ensure you add file_reader.cpp to your g++ command for Heap and Hash modules.

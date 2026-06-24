// *********************************************************
// File   : file_reader.h
// Course : CCP6214 Algorithm Design and Analysis
// Purpose: Shared CSV file-reading utilities for all
//          sorting and searching algorithm programs.
//          All team members #include this header.
// *********************************************************
//
//  Dataset CSV format (one record per line):
//    <10-digit integer>,<5-letter lowercase string>
//  Example:
//    1000000038,uoren
//    1000000009,igerk
//
//  Usage example:
//    #include "file_reader.h"
//    ...
//    long long n = 0;
//    Record* data = readCSV("dataset_1000.csv", n);
//    if (!data) { /* handle error */ }
//    // ... use data[0..n-1] ...
//    freeRecords(data);
// *********************************************************

#ifndef FILE_READER_H
#define FILE_READER_H

// ============================================================
//  Record  –  one entry in the dataset
// ============================================================
struct Record {
    long long key;  // 10-digit integer  (1,000,000,000 – 9,999,999,999)
    char      str[6]; // 5 lowercase letters + '\0' null terminator
};

// ============================================================
//  readCSV()
//    Opens 'filename', reads every non-empty line, parses
//    "integer,string" pairs and returns a heap-allocated
//    array of Records.
//
//  Parameters:
//    filename  – path to the CSV file
//    count     – [OUT] set to the number of records read
//
//  Returns:
//    Pointer to a new Record[] array (caller must free with
//    freeRecords()), or nullptr on error (count is set to 0).
//
//  Time complexity  : O(n)   where n = number of records
//  Space complexity : O(n)   for the returned array
// ============================================================
Record* readCSV(const char* filename, long long& count);

// ============================================================
//  freeRecords()
//    Frees the array previously returned by readCSV().
//    Safe to call with nullptr.
// ============================================================
void freeRecords(Record* records);

// ============================================================
//  printRecord()
//    Helper: print a single record in "integer/string" format.
//    Used by step-output files.
// ============================================================
void printRecord(const Record& r);

#endif // FILE_READER_H

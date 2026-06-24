// *********************************************************
// Program: radix_sort_step.cpp
// Course: CCP6214 Algorithm Design and Analysis
// *********************************************************
// Program: radix_sort_step.cpp
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
// Member_2 (RAZA): radix sort step
// *********************************************************

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Record {
    long long id;
    string text;
};

void printArray(ofstream& out,
                Record arr[],
                int size,
                string step) {

    out << "[";

    for (int i = 0; i < size; i++) {

        out << arr[i].id
            << "/"
            << arr[i].text;

        if (i < size - 1)
            out << ", ";
    }

    out << "] "
        << step
        << "\n";
}

void countSort(Record arr[],
               int size,
               long long exp) {

    Record* output = new Record[size];

    int count[10] = {0};

    for (int i = 0; i < size; i++) {
        count[(arr[i].id / exp) % 10]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (int i = size - 1; i >= 0; i--) {

        int digit = (arr[i].id / exp) % 10;

        output[count[digit] - 1] = arr[i];

        count[digit]--;
    }

    for (int i = 0; i < size; i++) {
        arr[i] = output[i];
    }

    delete[] output;
}

void radixSortStep(Record arr[],
                   int size,
                   ofstream& out) {

    printArray(out,
               arr,
               size,
               "original");

    long long exp = 1;

    for (int d = 10; d >= 1; d--) {

        countSort(arr,
                  size,
                  exp);

        printArray(out,
                   arr,
                   size,
                   "d=" + to_string(d));

        exp *= 10;
    }
}

int main() {



    string filename = "dataset_1000.csv";

    int startRow = 1;
    int endRow = 7;

    ifstream inFile(filename);

    if (!inFile) {
        cout << "Could not open file!"
             << endl;
        return 1;
    }

    vector<Record> tempList;

    string line;
    string str_id;
    string str_txt;

    int rowCount = 1;

    while (getline(inFile, line)
           && rowCount <= endRow) {

        if (rowCount >= startRow) {

            stringstream ss(line);

            getline(ss,
                    str_id,
                    ',');

            getline(ss,
                    str_txt);

            // Fix for hidden Windows carriage returns
            if (!str_txt.empty() && str_txt.back() == '\r') {
                str_txt.pop_back();
            }

            Record rec;

            rec.id = stoll(str_id);
            rec.text = str_txt;

            tempList.push_back(rec);
        }

        rowCount++;
    }

    inFile.close();

    int size = tempList.size();

    if (size == 0) {

        cout << "No data in that range."
             << endl;

        return 1;
    }

    Record* arr = new Record[size];

    for (int i = 0; i < size; i++) {
        arr[i] = tempList[i];
    }

    string outName =
        "dataset_1000_radix_sorted_step_"
        + to_string(startRow)
        + "_"
        + to_string(endRow)
        + ".txt";

    ofstream outFile(outName);

    outFile
        << "(processing from the rightmost character)\n";

    radixSortStep(arr,
                  size,
                  outFile);

    outFile.close();

    delete[] arr;

    cout << "Done. Check "
         << outName
         << endl;

    return 0;
}

// --------------------------
// Program: radix_sort.cpp
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
// Member_2 (RAZA): radix sort
// *********************************************************

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct Record {
    long long id;
    string text;
};

void countSort(Record arr[], int size, long long exp) {
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

void radixSort(Record arr[], int size) {
    long long maxID = arr[0].id;

    for (int i = 1; i < size; i++) {
        if (arr[i].id > maxID)
            maxID = arr[i].id;
    }

    for (long long exp = 1; maxID / exp > 0; exp *= 10) {
        countSort(arr, size, exp);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <dataset_n.csv>\n";
        return 1;
    }



    string in_file = string(argv[1]);

    // Extract the size text from filename, e.g. "dataset_1000.csv" -> "1000"
    string n_size = in_file;
    size_t p1 = in_file.find("dataset_");
    size_t p2 = in_file.find(".csv");
    if (p1 != string::npos && p2 != string::npos)
        n_size = in_file.substr(p1 + 8, p2 - p1 - 8);

    vector<Record> tempList;

    ifstream inFile(in_file);

    if (!inFile) {
        cout << "Could not open " << in_file << endl;
        return 1;
    }

    string line, str_id, str_txt;

    while (getline(inFile, line)) {
        stringstream ss(line);

        if (getline(ss, str_id, ',') && getline(ss, str_txt)) {

            // Fix for hidden Windows carriage returns
            if (!str_txt.empty() && str_txt.back() == '\r') {
                str_txt.pop_back();
            }

            Record rec;
            rec.id = stoll(str_id);
            rec.text = str_txt;

            tempList.push_back(rec);
        }
    }

    inFile.close();

    int size = tempList.size();

    Record* arr = new Record[size];

    for (int i = 0; i < size; i++) {
        arr[i] = tempList[i];
    }

    auto start = high_resolution_clock::now();

    radixSort(arr, size);

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);
    double time_taken = duration.count() / 1000000.0;

    string out_file = "radix_sorted_dataset_" + n_size + ".csv";

    ofstream outFile(out_file);

    for (int i = 0; i < size; i++) {
        outFile << arr[i].id << "/" << arr[i].text << "\n";
    }

    outFile << "\nRunning time: "
            << time_taken
            << " seconds\n";

    outFile.close();

    delete[] arr;

    cout << "Dataset size: "
         << size
         << endl;

    cout << "Running time: "
         << time_taken
         << " seconds"
         << endl;

    cout << "Saved to: "
         << out_file
         << endl;

    return 0;
}

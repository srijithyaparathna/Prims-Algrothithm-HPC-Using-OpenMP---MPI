#include <iostream>     // For console input/output
#include <fstream>      // For file I/O
#include <sstream>      // For parsing CSV lines
#include <vector>       // For using 2D vectors
#include <string>       // For strings
#include <map>          // For mapping version -> results
#include <tuple>        // For grouping multiple return values
#include <iomanip>      // For formatted output (e.g., setprecision)

using namespace std;

//  adjacency matrix from a CSV file
vector<vector<int>> read_csv(const string& filename) {
    ifstream fin(filename);
    vector<vector<int>> matrix;
    string line;

    while (getline(fin, line)) {
        vector<int> row;
        stringstream ss(line);
        string cell;

        while (getline(ss, cell, ',')) {
            row.push_back(stoi(cell));
        }

        matrix.push_back(row);
    }

    return matrix;
}

// const string& version

// accuracy (%) between reference and test matrices
double calculate_accuracy(const vector<vector<int>>& ref, const vector<vector<int>>& test) {
    int correct = 0, total = 0;

    for (size_t i = 0; i < ref.size(); i++) {
        for (size_t j = 0; j < ref[i].size(); j++) {
            if (ref[i][j] == test[i][j]) {
                correct++;
            }
            //  else {
            //     // print mismatches
            //     cout << "[Mismatch in " << version << "] at (" << i << "," << j 
            //          << ") → Serial=" << ref[i][j] 
            //          << ", " << version << "=" << test[i][j] << "\n";
            // }
            total++;
        }
    }

    return 100.0 * correct / total;
}

// Parses execution time log file ("log_time.txt") to extract:
//    version name, number of vertices, thread/process count, and execution time
map<string, tuple<int, int, double>> parse_log(const string& filename) {
    map<string, tuple<int, int, double>> log_map;
    ifstream fin(filename);
    string line;

    while (getline(fin, line)) {
        stringstream ss(line);
        string version;
        int vertices, tp;
        double time;

        getline(ss, version, ','); // version
        ss >> vertices; ss.ignore();  // skip comma
        ss >> tp; ss.ignore();        // skip comma
        ss >> time;

        log_map[version] = {vertices, tp, time};
    }

    return log_map;
}

int main() {
    //  Input and Output File Names
    string serial_file = "serial_output.csv";  // Reference MST from serial implementation
    vector<string> versions = {"OpenMP", "MPI", "MPI+OpenMP"}; // Comparison targets
    vector<string> files = {"openmp_output.csv", "mpi_output.csv", "hybrid_output.csv"};

    //  Load the serial MST as reference
    vector<vector<int>> serial_mst = read_csv(serial_file);

    //  Load MSTs from other implementations
    map<string, vector<vector<int>>> other_msts;
    for (size_t i = 0; i < versions.size(); i++) {
        other_msts[versions[i]] = read_csv(files[i]);
    }

    // timing data from log_time.txt
    map<string, tuple<int, int, double>> log_data = parse_log("log_time.txt");
    double serial_time = get<2>(log_data["Serial"]);

    //  Output CSV file for accuracy + time comparison
    ofstream fout("accuracy_comparison.csv");
    fout << "Version,Vertices,Threads/Processes,Accuracy (%),Time (s),Time Difference (s)\n";

    //  Compare each version against serial MST and write results
    for (const auto& [version, mst] : other_msts) {
        int vertices = get<0>(log_data[version]);
        int threads_or_procs = get<1>(log_data[version]);
        double time = get<2>(log_data[version]);


      // double accuracy = calculate_accuracy(serial_mst, mst, version);


       double accuracy = calculate_accuracy(serial_mst, mst);
        double time_diff = time - serial_time;

        fout << version << "," << vertices << "," << threads_or_procs << ",";
        fout << fixed << setprecision(2) << accuracy << "," << time << "," << time_diff << "\n";
    }

    fout.close();
    cout << " Accuracy and time comparison written to accuracy_comparison.csv\n";

    return 0;
}

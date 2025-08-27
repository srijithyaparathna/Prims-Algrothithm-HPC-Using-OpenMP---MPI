#include <iostream>    // for standard input and output
#include <fstream>     // for file handling
#include <vector>      // for dynamic vector
#include <limits>      // for INF constant
#include <omp.h>       // for OpenMP parallelism

#define INF 9999       // Represent infinity for Prim's algorithm

using namespace std;

int main() {
    string input_file = "input.csv";             // Input file
    string output_file = "openmp_output.csv";    // Output file

    ifstream fin(input_file);    // Open the input CSV file
    ofstream fout(output_file); // Open the output CSV file

    if (!fin.is_open()) {
        cerr << "Error: cannot open input file " << input_file << "\n";
        return 1;
    }

    // Initialize the 2D vector (graph)
    vector<vector<int>> graph;
    string line;

    // Read the adjacency matrix from CSV
    while (getline(fin, line)) {
        vector<int> row;
        size_t pos = 0;

        while ((pos = line.find(',')) != string::npos) {
            row.push_back(stoi(line.substr(0, pos)));
            line.erase(0, pos + 1);
        }

        row.push_back(stoi(line)); // Add the last number in the row
        graph.push_back(row);
    }

    int V = graph.size(); // Number of vertices

    vector<int> parent(V, -1);      // Store MST tree
    vector<int> key(V, INF);        // Store minimum edge weights
    vector<bool> inMST(V, false);   // Tracks visited nodes

    key[0] = 0; // Start from the vertex 0

    double start = omp_get_wtime(); 

    // Repeat for V vertices
    for (int count = 0; count < V; count++) {
        int min_key = INF;
        int u = -1;

        // Find the minimum key vertex not yet included in MST
        #pragma omp parallel
        {
            int local_min = INF;
            int local_u = -1;

            #pragma omp for nowait
            for (int v = 0; v < V; v++) {
                if (!inMST[v] && key[v] < local_min) {
                    local_min = key[v];


                         local_u = v;
                }
            }

            #pragma omp critical
            {
                if (local_min < min_key) {
                      min_key = local_min;
                    u = local_u;
                }
            }
        }

        if (u == -1) break;

        inMST[u] = true; 

        #pragma omp parallel for
        for (int v = 0; v < V; v++) {
            if (graph[u][v] && !inMST[v] && graph[u][v] < key[v]) {
                key[v] = graph[u][v];
                parent[v] = u;
            }
        }
    }


  vector<vector<int>> mst(V, vector<int>(V, INF));

    for (int i = 1; i < V; i++) {
        int u = parent[i], v = i;
        mst[u][v] = graph[u][v];
        mst[v][u] = graph[v][u];  
    }

    //  Write MST to output CSV
    for (const auto& row : mst) {
        for (size_t j = 0; j < row.size(); j++) {
            fout << row[j];
            if (j < row.size() - 1) fout << ",";
        }
        fout << "\n";
    }

    double end = omp_get_wtime();         
    double duration = end - start;


    ofstream log("log_time.txt", ios::app);
    log << "OpenMP," << V << "," << omp_get_max_threads() << "," << duration << "\n";
    log.close();


    cout << "Execution time: " << duration << " seconds\n";
    cout << "Number of threads used: " << omp_get_max_threads() << "\n";
    cout << "Output is ready in file: " << output_file << "\n";

    return 0;
}

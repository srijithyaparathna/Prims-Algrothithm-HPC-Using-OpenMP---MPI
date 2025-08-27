#include <iostream>  // For input/output operations
#include <fstream>   // For file operations
#include <vector>    // For dynamic 2D vector (adjacency matrix)
#include <limits>    // For defining INF (infinite) values
#include <chrono>    // For measuring execution time

using namespace std;
using namespace std::chrono; // to use high_resolution_clock and duration

#define INF 9999  // Define a large value to represent infinity (no edge)

int main()
{
    // Input and output file names (fixed)
    string input_file = "input.csv";           
    string output_file = "serial_output.csv";  

    ifstream fin(input_file);      // Open input file stream
    ofstream fout(output_file);    // Open output file stream

    // Check if input file is open successfully
    if (!fin.is_open())
    {
        cerr << "Error: Could not open " << input_file << endl;
        return 1;  // Exit with error
    }
    
    // 2D vector to store graph adjacency matrix
    vector<vector<int>> graph;
    string line;

    // Read CSV file line by line and convert each line to a vector<int>
    while (getline(fin, line))
    {
        vector<int> row;
        size_t pos = 0;

        while ((pos = line.find(',')) != string::npos )
        {
            row.push_back(stoi(line.substr(0, pos)));
            line.erase(0, pos + 1);                    
        }

       
        row.push_back(stoi(line)); 

   
        graph.push_back(row);
    }
    fin.close();  

    int V = graph.size(); 

    // Initialize arrays for Prim's algorithm:
    vector<int> parent(V, -1);      // parent[i] = vertex that connects i in MST
    vector<int> key(V, INF);         // key[i] = min weight edge to include vertex i
    vector<bool> inMST(V, false);    // inMST[i] = true if vertex i included in MST

    key[0] = 0; 

    // Start measuring time before MST computation
    auto start = high_resolution_clock::now();

    // Prim's algorithm main loop: run V-1 times to add edges to MST
    for (int count = 0; count < V - 1; count++)
    {
        int min = INF, u = -1;

        // Find vertex u not in MST with minimum key value
        for (int v = 0; v < V; v++) {
            if (!inMST[v] && key[v] < min)
            {
                min = key[v];
                u = v;
            }
        }

        inMST[u] = true; 

        
        for (int v = 0; v < V; v++)
        {
            // If there is an edge u->v, v not in MST, and weight less than current key[v]
            if (graph[u][v] && !inMST[v] && graph[u][v] < key[v])
            {
                parent[v] = u;      // Set u as parent of v in MST
                key[v] = graph[u][v]; // Update minimum weight to include v
            }
        }
    }

    // Create adjacency matrix for MST initialized with zeros
vector<vector<int>> mst(V, vector<int>(V, INF));


    // Fill MST matrix using parent array
    for (int i = 1; i < V; i++) {
        int u = parent[i], v = i;
        mst[u][v] = graph[u][v];
        mst[v][u] = graph[v][u];  
    }

    // Write MST adjacency matrix to output CSV file
    for (const auto& row : mst) {
        for (size_t j = 0; j < row.size(); j++) {
            fout << row[j];
            if (j < row.size() - 1) fout << ",";
        }
        fout << "\n";
    }
    fout.close(); // Close output file

    // End timing after MST computation
    auto end = high_resolution_clock::now();
    double duration = duration_cast<std::chrono::duration<double>>(end - start).count();


 
    ofstream log("log_time.txt", ios::app);
    log << "Serial," << V << ",1," << duration << "\n";
    log.close();

cout << "Execution time: " << duration << " seconds\n";
cout << "Output is ready in file: " << output_file << "\n";

    return 0;
}

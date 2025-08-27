#include <mpi.h>       // MPI library for inter-process communication
#include <omp.h>       // OpenMP for multi-threading within each process
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

#define INF 9999        // A large number representing 'infinity'

using namespace std;

// Function to read adjacency matrix of graph from CSV file
vector<vector<int>> read_csv(const string& filename) {
    ifstream fin(filename);
    string line;
    vector<vector<int>> matrix;

    // Read each line and convert comma-separated numbers into a row
    while (getline(fin, line)) {
        vector<int> row;
        size_t pos;
        while ((pos = line.find(',')) != string::npos) {
            row.push_back(stoi(line.substr(0, pos)));
            line.erase(0, pos + 1);
        }
        row.push_back(stoi(line)); // Last number after last comma
        matrix.push_back(row);
    }
    return matrix;
}

// Function to write adjacency matrix to CSV file
void write_csv(const vector<vector<int>>& mat, const string& filename) {
    ofstream fout(filename);
    for (const auto& row : mat) {
        for (size_t j = 0; j < row.size(); j++) {
            fout << row[j];
            if (j < row.size() - 1) fout << ",";
        }
        fout << "\n";
    }
}

int main(int argc, char** argv) {
    // Fixed file names
    string input_file = "input.csv";





    string output_file = "hybrid_output.csv";

    MPI_Init(&argc, &argv); // Start MPI environment

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get current process rank
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get total number of processes

    vector<vector<int>> graph; // Adjacency matrix
    int V; // Number of vertices

    if (rank == 0) {
        graph = read_csv(input_file); 
        V = graph.size();             
    }

    
    MPI_Bcast(&V, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // All other processes resize their graph
    if (rank != 0) graph.resize(V, vector<int>(V));

    // Broadcast the entire graph to all processes
    for (int i = 0; i < V; i++)
        MPI_Bcast(graph[i].data(), V, MPI_INT, 0, MPI_COMM_WORLD);

  
    vector<int> parent(V, -1);    

    vector<int> key(V, INF);   

    vector<bool> inMST(V, false); 
    key[0] = 0;                    // Start from vertex 0

    double start = MPI_Wtime(); // Start timing

    // Repeat until all vertices are included in MST
    for (int count = 0; count < V - 1; count++) {
        int local_min = INF, u = -1;

        // Step 1: Local process finds minimum key vertex not in MST
        #pragma omp parallel
        {
            int tmin = INF, tu = -1;

            // Divide vertex search among OpenMP threads and MPI processes
            #pragma omp for nowait
            for (int i = rank; i < V; i += size) {
                if (!inMST[i] && key[i] < tmin) {
                    tmin = key[i];
                    tu = i;
                }
            }

            // Use critical section to update process-local minimum
            #pragma omp critical
            {
                if (tmin < local_min) {
                    local_min = tmin;
                    u = tu;
                }
            }
        }

        // Step 2: Global reduction to find process with global minimum key
        struct {
            int val;
            int idx;
        } local_pair = {local_min, u}, global_pair;

        MPI_Allreduce(&local_pair, &global_pair, 1, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD);
        u = global_pair.idx;      // Chosen global minimum vertex
        inMST[u] = true;          // Mark it included

        // Step 3: Update key and parent arrays in parallel
        #pragma omp parallel for
        for (int v = 0; v < V; v++) {
          
            if (graph[u][v] && !inMST[v] && graph[u][v] < key[v]) {
                            key[v] = graph[u][v];
                parent[v] = u;
            }
        }
    }



    // the MST to output file
    if (rank == 0) {
        // Construct the MST as adjacency matrix
       vector<vector<int>> mst(V, vector<int>(V, INF));

        for (int i = 1; i < V; i++) {
            int u = parent[i], v = i;
            mst[u][v] = graph[u][v];
            mst[v][u] = graph[v][u]; // Undirected graph
        }

        // Save MST matrix to CSV file
        write_csv(mst, output_file);

        double end = MPI_Wtime();
        double duration = end - start;

       int total_threads = size * omp_get_max_threads();
        // Log performance results
        ofstream log("log_time.txt", ios::app);
        log << "MPI+OpenMP," << V << "," << total_threads << "," << duration << "\n";
        log.close();



cout << "Execution time: " << duration << " seconds" << endl;
cout << "Vertices: " << V << endl;

cout << "Processes used (MPI): " << size << endl;

cout << "Threads per process (OpenMP): " << omp_get_max_threads() << endl;
cout << "Output saved to: " << output_file << endl;



    }

    MPI_Finalize(); 
    return 0;
}

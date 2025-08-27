#include <mpi.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

#define INF 9999

using namespace std;
// File paths (update these if needed)
const string INPUT_FILE = "input.csv";
const string OUTPUT_FILE = "mpi_output.csv";
const string LOG_FILE = "log_time.txt";

// Reads a CSV file into a 2D adjacency matrix
vector<vector<int>> read_csv(const string& filename) {
    ifstream fin(filename);
    string line;
    vector<vector<int>> matrix;

    while (getline(fin, line)) {
        vector<int> row;
        size_t pos;
        while ((pos = line.find(',')) != string::npos) {
            row.push_back(stoi(line.substr(0, pos)));
            line.erase(0, pos + 1);
        }
        row.push_back(stoi(line));  // Add the last element
        matrix.push_back(row);
    }
    return matrix;
}


// Writes a 2D matrix to a CSV file
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

int main(int argc, char** argv){
// Initialize MPI environment
 MPI_Init(&argc, &argv);

  int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Get current process ID
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Get total number of processes

    vector<vector<int>> graph;
    int V; // Number of vertices

  // Rank 0 reads the input file and broadcasts to others
    if (rank == 0) {
        graph = read_csv(INPUT_FILE);
        V = graph.size();
    }


    // Broadcast graph size to all processes
    MPI_Bcast(&V,1,MPI_INT,0,MPI_COMM_WORLD);


    if (rank !=0)
    {
        graph.resize(V,vector<int>(V));
    }
    

    // Broadcast the actual matrix row-by-row
    for (int i = 0; i < V; i++)
        MPI_Bcast(graph[i].data(),V,MPI_INT,0,MPI_COMM_WORLD);

    // Initialize Prims algorithm data structures

    // Stores MST structure
    vector<int> parent(V,-1);

    // Minimum weights
    vector<int> key(V,INF);

    //check MST 
    vector<bool> inMST(V,false);

    key[0] = 0;

    // start time measurement
    double start = MPI_Wtime();
    


for (int count = 0; count < V - 1; count++) {
        int local_min = INF, u = -1;

        // Each process checks its own subset of vertices
        for (int i = rank; i < V; i += size) {
            if (!inMST[i] && key[i] < local_min) {
                local_min = key[i];
                u = i;
            }
        }

        // Find the global minimum vertex not in MST
        struct {
            int val;
            int idx;
        } local_pair = {local_min, u}, global_pair;

        MPI_Allreduce(&local_pair, &global_pair, 1, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD);

        u = global_pair.idx;
        inMST[u] = true;

        // Update key and parent of the adjacent vertices
        for (int v = 0; v < V; v++) {
            if (graph[u][v] && !inMST[v] && graph[u][v] < key[v]) {
                key[v] = graph[u][v];
                parent[v] = u;
            }
        }
    }

 // Only rank 0 writes the result
    if (rank == 0) {
        vector<vector<int>> mst(V, vector<int>(V, INF));


        // Construct MST matrix from parent array
        for (int i = 1; i < V; i++) {
            int u = parent[i], v = i;
            mst[u][v] = graph[u][v];
            mst[v][u] = graph[v][u];
        }

        // Write MST to output file
        write_csv(mst, OUTPUT_FILE);

        // End time and log
        double end = MPI_Wtime();
        double duration = end - start;

        // Log format: MPI,Vertices,Processes,Time
        ofstream log(LOG_FILE, ios::app);
        log << "MPI," << V << "," << size << "," << duration << "\n";
        log.close();
          cout << "Execution time: " << duration << " seconds\n";
    cout << "Number of processes: " << size << "\n";
    cout << "Output file: " << OUTPUT_FILE << "\n";
    }

    // Finalize MPI
    MPI_Finalize();
    return 0;







}
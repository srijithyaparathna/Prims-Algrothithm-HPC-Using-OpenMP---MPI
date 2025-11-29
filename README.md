# Parallel Minimum Spanning Tree (Prim's Algorithm) -- Serial, OpenMP, MPI, Hybrid

This project implements **Prim's Algorithm** for generating the
**Minimum Spanning Tree (MST)** of a graph using four different
computational models:

-   **Serial (C++)**
-   **OpenMP (Shared-memory parallelism)**
-   **MPI (Distributed-memory parallelism)**
-   **Hybrid MPI + OpenMP (Distributed + Shared)**

It also includes a **1000×1000 adjacency matrix generator** to create
large test inputs.

------------------------------------------------------------------------

## 🚀 Features
<img width="1050" height="1649" alt="Untitled diagram-2025-11-29-105740" src="https://github.com/user-attachments/assets/fddff47e-95a9-4443-bf52-95eb623f2901" />

### ✔ Serial Implementation

Simple C++ implementation of Prim's algorithm.

### ✔ OpenMP Version
<img width="1681" height="776" alt="Untitled diagram-2025-11-29-121447" src="https://github.com/user-attachments/assets/73510cbc-eb31-4b14-ae29-16e1c60b4508" />


Uses multithreading to parallelize MST vertex selection and key updates.

### ✔ MPI Version

Parallelizes Prim's algorithm across distributed nodes using message
passing.

### ✔ Hybrid MPI + OpenMP

Combines MPI for process-level parallelism and OpenMP for thread-level
parallelism inside each node.

### ✔ Input Generator

Creates a **1000×1000 adjacency matrix** with:\
- `0` on diagonal\
- `NO_EDGE (9999)` for no connection\
- Random weights (1--100)\
- Symmetric structure

Output saved in `input.csv`.

<img width="1238" height="882" alt="Untitled diagram-2025-11-29-121549" src="https://github.com/user-attachments/assets/0ca401c2-5266-4b56-a768-751e94434600" />


------------------------------------------------------------------------

## 📂 File Structure

    |-- input_generator.cpp
    |-- serial.cpp
    |-- openmp.cpp
    |-- mpi.cpp
    |-- hybrid_mpi_openmp.cpp
    |-- input.csv
    |-- serial_output.csv
    |-- openmp_output.csv
    |-- mpi_output.csv
    |-- hybrid_output.csv
    |-- log_time.txt
    |-- README.md

------------------------------------------------------------------------

<img width="2214" height="882" alt="Untitled diagram-2025-11-29-121620" src="https://github.com/user-attachments/assets/67965bd2-34bc-49e7-a90d-c8d7bb56fd15" />



## 🧪 How to Run

### 1️⃣ Generate Input

    g++ input_generator.cpp -o gen
    ./gen

------------------------------------------------------------------------

### 2️⃣ Serial Version

    g++ serial.cpp -o serial
    ./serial

------------------------------------------------------------------------

### 3️⃣ OpenMP Version

    g++ -fopenmp openmp.cpp -o openmp
    export OMP_NUM_THREADS=8
    ./openmp

------------------------------------------------------------------------

### 4️⃣ MPI Version

    mpic++ mpi.cpp -o mpi
    mpirun -np 4 ./mpi

------------------------------------------------------------------------

### 5️⃣ Hybrid MPI + OpenMP Version

    mpic++ -fopenmp hybrid.cpp -o hybrid
    export OMP_NUM_THREADS=4
    mpirun -np 2 ./hybrid

------------------------------------------------------------------------


<img width="1175" height="1198" alt="Untitled diagram-2025-11-29-121651" src="https://github.com/user-attachments/assets/65fe4bbe-0d2c-49d5-8286-4a1052bf9844" />



## 📊 Performance Logs

All execution times are appended automatically into:

    log_time.txt

Format:

    Model,Vertices,Threads/Processes,Time
    Serial,1000,1,3.552
    OpenMP,1000,8,0.812
    MPI,1000,4,0.674
    MPI+OpenMP,1000,16,0.291

------------------------------------------------------------------------

## 🛠 Requirements

### For Serial & OpenMP

-   GCC/G++

### For MPI

-   OpenMPI or MPICH

### For Hybrid

-   OpenMPI + GCC with OpenMP

------------------------------------------------------------------------

## 📘 Output

Each implementation generates:

-   A CSV MST adjacency matrix\
-   Execution time printed on console\
-   Entry in `log_time.txt`

Example file:

    serial_output.csv
    openmp_output.csv
    mpi_output.csv
    hybrid_output.csv

------------------------------------------------------------------------

## 📝 Notes

-   The graph may contain many `INF (9999)` values → sparse graph.
-   Computation time increases with matrix size.
-   Hybrid version is usually fastest.

------------------------------------------------------------------------

## 📄 License

This project is free to use for academic and research purposes.

------------------------------------------------------------------------

## 👨‍💻 Author

Srijith Yaparathne\
Parallel Programming \| HPC \| C++ \| MPI \| OpenMP

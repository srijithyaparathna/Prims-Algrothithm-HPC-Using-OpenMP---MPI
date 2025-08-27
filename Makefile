# Compiler settings
CXX = g++
MPICXX = mpic++

CXXFLAGS = -O2 -std=c++17

OPENMP_FLAGS = -fopenmp

# Source files
SERIAL_SRC = prim_serial.cpp
OPENMP_SRC = openmp_prim.cpp
MPI_SRC = mpi_prim.cpp
HYBRID_SRC = hybrid_prim.cpp
COMPARE_SRC = time_accuracy_comparison.cpp

# Executable names
SERIAL_EXE = prim_serial
OPENMP_EXE = openmp_prim
MPI_EXE = mpi_prim
HYBRID_EXE = hybrid_prim
COMPARE_EXE = time_accuracy_comparison

.PHONY: all clean run_serial run_openmp run_mpi run_hybrid run_compare

all: $(SERIAL_EXE) $(OPENMP_EXE) $(MPI_EXE) $(HYBRID_EXE) $(COMPARE_EXE)

$(SERIAL_EXE): $(SERIAL_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OPENMP_EXE): $(OPENMP_SRC)
	$(CXX) $(CXXFLAGS) $(OPENMP_FLAGS) -o $@ $<

$(MPI_EXE): $(MPI_SRC)
	$(MPICXX) $(CXXFLAGS) -o $@ $<

$(HYBRID_EXE): $(HYBRID_SRC)
	$(MPICXX) $(CXXFLAGS) $(OPENMP_FLAGS) -o $@ $<

$(COMPARE_EXE): $(COMPARE_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(SERIAL_EXE) $(OPENMP_EXE) $(MPI_EXE) $(HYBRID_EXE) $(COMPARE_EXE) *.o

# Default number of processes and threads
PROCS ?= 4
THREADS ?= 4

run_serial: $(SERIAL_EXE)
	./$(SERIAL_EXE)

run_openmp: $(OPENMP_EXE)
	OMP_NUM_THREADS=$(THREADS) ./$(OPENMP_EXE)

run_mpi: $(MPI_EXE)
	mpirun --allow-run-as-root -np $(PROCS) ./$(MPI_EXE)

run_hybrid: $(HYBRID_EXE)
	OMP_NUM_THREADS=$(THREADS) mpirun --allow-run-as-root -np $(PROCS) ./$(HYBRID_EXE)

run_compare: $(COMPARE_EXE)
	./$(COMPARE_EXE)

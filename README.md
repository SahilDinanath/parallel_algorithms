# Parallel Algorithms 

![intro](https://github.com/SahilDinanath/parallel_algorithms/assets/46680594/4a304160-de69-4ca4-87b3-0db015f6acfe)

## Table of Contents
- [Introduction](#introduction)
- [Algorithms](#algorithms)
  - [Prefix Scan](#prefix-scan)
  - [Bitonic Sort](#bitonic-sort)
  - [Dijkstra's SSSP](#dijkstras-sssp)
- [Implementations](#implementations)
  - [Serial](#serial)
  - [OpenMP Parallel](#openmp-parallel)
  - [MPI Parallel](#mpi-parallel)
- [Report](#report)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Introduction

Parallel computing plays a crucial role in addressing the growing demand for increased computational power. This repository focuses on parallel implementations of three classical algorithms - Prefix Scan, Bitonic Sort, and Dijkstra's Single Source Shortest Path. The goal is to explore and compare the performance of these algorithms under different parallelization paradigms. Each algorithm is implemented in three different paradigms - serial, OpenMP (omp) parallel, and Message Passing Interface (MPI) parallel. 

## Report
The report goes into the introduction of each algorithm, includes implementation details, comprehensive analysis, charts, and insights derived from the benchmarking process. The report can be found [here](report/report.pdf). 

## Algorithms

### Prefix Scan

The Prefix Scan algorithm is used to efficiently compute the prefix sum of elements in an array.

### Bitonic Sort

Bitonic Sort is a parallel sorting algorithm that can efficiently sort sequences of data.

### Dijkstra's SSSP

Dijkstra's Single Source Shortest Path algorithm is employed to find the shortest paths from a source vertex to all other vertices in a weighted graph.

## Implementations

### Serial

The serial implementations serve as baseline reference implementations for each algorithm.

### OpenMP Parallel

OpenMP is employed to introduce parallelism to the algorithms, allowing for shared-memory parallelization.

### MPI Parallel

Message Passing Interface (MPI) is utilized to parallelize the algorithms across distributed-memory systems.


## Usage

To use and experiment with these parallel algorithms, follow the instructions provided in each algorithm's directory. The bash script `(run.sh)` in the top directory of the project will execute all scripts for each implementation.

### Requirements
### OpenMP (omp)

1. **GCC (GNU Compiler Collection):**
   
   Install the GCC compiler, which includes support for OpenMP.

   ```bash
   sudo apt-get update
   sudo apt-get install build-essential
   ```

2. **Libomp (OpenMP Runtime Library):**

   Install the libomp library for OpenMP support.

   ```bash
   sudo apt-get install libomp-dev
   ```

### MPI (Message Passing Interface)

1. **Open MPI:**

   Install the Open MPI implementation.

   ```bash
   sudo apt-get update
   sudo apt-get install openmpi-bin libopenmpi-dev
   ```
OR

2. **MPICH:**

   Install the MPICH implementation.

   ```bash
   sudo apt-get update
   sudo apt-get install mpich
   ```

### Python

 **Python and Pip:**

   Ensure that Python and pip are installed on your system.

   ```bash
   sudo apt-get update
   sudo apt-get install python3
   ```

Make sure that the versions and installations are consistent with your system requirements. Adjust the package names and versions based on your system and preferences.

#### Line endings consistency
```bash
sudo apt install dos2unix
```
The `dos2unix` command is useful for converting text files between different line ending formats (e.g., Windows CRLF to Unix LF). Running the above command with `sudo` will install `dos2unix` using `apt` package manager on Debian-based systems. Make sure you have appropriate permissions to install packages using `apt`.

Note: only necessary if you get an interpreter error
Uncomment the `unix.sh` in the top directory's `run.sh` in order to execute this on all necessary files.

### Parallel Scan
To execute with different input sizes edit the `power_of_two` variable in `scan/run.sh`.
The amount of threads or processes can be changed using the variables located at the top of the run script. 
### Bitonic
To execute with different input sizes do the following:
The amount of threads or processes can be changed using the variables located at the top of the run script. 
1. Edit `bitonicsort/randomarrays.py` and change `<input_size>` in the following two lines
```py
file = open("input_2_<input_size>.txt","w")
n = 2 ** <input_size>  # Size of the bitonic sequence
```
2. Edit `bitonicsort/run.sh` 
```bash
file_name="input_2_<input_size>.txt"
```
3. Run `randomarrays.py` beforehand if the `<input_size>` is greater than 22
### Dijkstra SSSP
To execute with different graphs change `<graph_num>` in ``sssp_dijkstra/run.sh``.
For the Sequential Scan correctness of the algorithm is displayed when tested with the graph_0.txt.
The amount of threads or processes can be changed using the variables located at the top of the run script. 
```bash
file_name="graph_<graph_num>.txt"
```

## Contributing

Contributions are welcome! If you have improvements, bug fixes, or new algorithms to add, please open an issue or submit a pull request.

## License

This project is licensed under the GNU General Public License v2.0 - see the [LICENSE](LICENSE) file for details.

# Parallel Algorithms 
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
- [Benchmarking](#benchmarking)
- [Report](#report)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Introduction

Parallel computing plays a crucial role in addressing the growing demand for increased computational power. This repository focuses on parallel implementations of three classical algorithms - Prefix Scan, Bitonic Sort, and Dijkstra's Single Source Shortest Path. The goal is to explore and compare the performance of these algorithms under different parallelization paradigms. Each algorithm is implemented in three different paradigms - serial, OpenMP (omp) parallel, and Message Passing Interface (MPI) parallel.

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

## Benchmarking

The performance of each algorithm and implementation is benchmarked to evaluate the efficiency of the parallelization strategies. The benchmarking results are recorded and analyzed in detail in the report.

## Report

The detailed benchmarking report can be found in the [Report](report/) directory. It includes comprehensive analysis, charts, and insights derived from the benchmarking process.

## Usage

To use and experiment with these parallel algorithms, follow the instructions provided in each algorithm's directory. The bash script `(run.sh)` in the top directory of the project will execute all scripts for each implementation.

### Requirements
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

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

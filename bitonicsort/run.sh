#!/bin/bash

# Serial implementation
echo "Running serial bitonic sort:"
./bitonic

# OpenMP implementation
echo "Running OpenMP bitonic sort:"
./bitonic_omp

# MPI implementation
echo "Running MPI bitonic sort:"
mpiexec -n 4 ./bitonic_mpi

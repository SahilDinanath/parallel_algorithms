#!/bin/bash

num_of_runs=100

make clean
make all 

echo "========================"
echo "Results of Scan Implementations"
echo "========================"
echo "scan:"
./scan
echo ""
echo "scan_omp:"
./scan_omp
echo ""
echo "========================"

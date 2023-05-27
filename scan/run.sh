#!/bin/bash

num_of_runs=100
num_of_threads = 10

make clean
make all 

echo "========================"
echo "Results of Scan Implementations"
echo "========================"
echo "scan:"
#the parameters that scan take are ./scan <file_name> <number_of_digits_of_file>
./scan input_2_16.txt 65536
echo ""
echo "scan_omp:"
#the parameters that scan_omp take are ./scan_omp <file_name> <number_of_digits_of_file> <threads>
./scan_omp input_2_16.txt 65536 4
echo ""
echo "========================"

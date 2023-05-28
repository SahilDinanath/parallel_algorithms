#!/bin/bash

num_of_runs=100

make clean
make all 

echo "========================"
echo "Results of Scan Implementations"
echo "========================"
echo "scan:"
#the parameters that scan take are ./scan <file_name> <number_of_digits_of_file>
# ./scan input_2_32.txt 4294967296
./scan input_2_28.txt 268435456 
# ./scan input_2_26.txt 67108864
# ./scan input_2_24.txt 16777216
# ./scan input_2_16.txt 65536
echo ""
echo "scan_omp:"
#the parameters that scan_omp take are ./scan_omp <file_name> <number_of_digits_of_file> <threads>
./scan_omp input_2_28.txt 268435456 2
# ./scan_omp input_2_26.txt 67108864 8
# ./scan_omp input_2_36.txt 4294967296 4
# ./scan_omp input_2_24.txt 16777216 4
# ./scan_omp input_2_16.txt 65536 4
echo ""
echo "========================"

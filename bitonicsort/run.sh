#!/bin/bash

# Options for running files
file_name="input_2_3.txt"
threads=4
processes=4

make clean
make all 

echo "========================"
echo "Results of Bitonic Sort Implementations"
echo "========================"

echo "bitonic:"
for i in {1..4} 
do
    ./bitonic "${file_name}" | tail -n 1 >> times.txt
done 
# cat times.txt
python3 average.py
> times.txt

echo "bitonic_omp:"
for i in {1..4} 
do
    ./bitonic_omp "${file_name}" "${threads}" | tail -n 1 >> times.txt
done  
# cat times.txt
python3 average.py
> times.txt

echo "bitonic_mpi:"
for i in {1..4} 
do
    mpirun -np "${processes}" ./bitonic_mpi "${file_name}" | tail -n 1 >> times.txt
done 
# cat times.txt
python3 average.py
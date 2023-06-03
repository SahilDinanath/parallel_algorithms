#!/bin/bash

# Options for running files
file_name="input_2_16.txt"
threads=4
processes=4

make clean
make all 

echo "========================"
echo "Results of Bitonic Sort Implementations"
echo "========================"
# only uncomment when using <18
python3 randomarrays.py
> times.txt
> speedup.txt
echo "bitonic:"
for i in {1..4} 
do
    output=$(./bitonic ${file_name}) 
    echo "$output"
    echo "$output" | tail -n 1 >> times.txt
done 
# cat times.txt
output=$(python3 average.py)
echo "$output"
echo "$output" >> speedup.txt
> times.txt

echo "bitonic_omp:"
for i in {1..4} 
do
    output=$(./bitonic_omp ${file_name})
    echo "$output"
    echo "$output" | tail -n 1 >> times.txt
done  
# cat times.txt
output=$(python3 average.py)
echo "$output"
echo "$output" >> speedup.txt
> times.txt

echo "bitonic_mpi:"
for i in {1..4} 
do
    output=$(mpirun -np "${processes}" ./bitonic_mpi ${file_name})
    echo "$output"
    echo "$output" | tail -n 1 >> times.txt
done 
# cat times.txt
output=$(python3 average.py)
echo "$output"
echo "$output" >> speedup.txt
> times.txt

echo "========================"
echo "Average Speedups"
echo "========================"

# Calculate speedup between the first average time and the other two times
python3 speedup.py
> speedup.txt

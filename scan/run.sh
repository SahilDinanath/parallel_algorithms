#!/bin/bash

# options for running files
power_of_two=18
threads=4
processes=4

make clean
make all

echo "========================"
echo "Results of Scan Implementations"
echo "========================"
echo "scan:"

# Run scan four times and append the last line of output to times.txt
for i in {1..4}
do
    output=$(./scan ${power_of_two})
    echo "$output"
    echo "$output" | tail -n 1 >> times.txt
done
out=$(python3 average.py)
echo "$out"
echo "$out" >> speedup.txt
> times.txt

echo ""
echo "scan_omp:"

# Run scan_omp four times and append the last line of output to times.txt
for i in {1..4}
do
    output=$(./scan_omp ${power_of_two} ${threads})
    echo "$output"
    echo "$output" | tail -n 1 >> times.txt
done
out=$(python3 average.py)
echo "$out"
echo "$out" >> speedup.txt
> times.txt

echo ""
echo "scan_mpi:"

# Run scan_mpi four times and append the last line of output to times.txt
for i in {1..4}
do
    output=$(mpirun -np ${processes} ./scan_mpi ${power_of_two} ${processes})
    echo "$output"
    echo "$output" | tail -n 1 >> times.txt
done
out=$(python3 average.py)
echo "$out"
echo "$out" >> speedup.txt
> times.txt

echo ""
echo "========================"

# Calculate speedup between the first average time and the other two times
python3 speedup.py
> speedup.txt
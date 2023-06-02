#!/bin/bash

# options for running files
power_of_two=18
threads=2
processes=2

make clean
make all

echo "========================"
echo "Results of Scan Implementations"
echo "========================"

echo "scan:"
for i in {1..4}
do
    ./scan ${power_of_two} | tee -a times.txt
done

python3 calculate_average_speedup.py
> times.txt

echo "scan_omp:"
for i in {1..4}
do
    ./scan_omp ${power_of_two} ${threads} | tee -a times.txt
done

python3 calculate_average_speedup.py
> times.txt

echo "scan_mpi:"
for i in {1..4}
do
    mpirun -np ${processes} ./scan_mpi ${power_of_two} ${processes} | tee -a times.txt
done

python3 calculate_average_speedup.py
> times.txt
> speedup.txt



#!/bin/bash

# Options for running files
# file_name="input_2_20.txt"
power_of_two=28
threads=4
processes=6

make clean
make all 



echo "========================"
echo "Results of Bitonic Sort Implementations"
echo "========================"
echo ""
# Extract the value of num from the file_name
# num=$(echo "$file_name" | sed -n 's/input_2_\([0-9]*\)\.txt/\1/p')

# if (( power_of_two <= 22 )); then
#   python3 randomarrays.py
# else
#   echo "Reading from pre-generated file"
# fi
> times.txt
> speedup.txt
echo ""
echo "bitonic:"
for i in {1..4} 
do
    output=$(./bitonic ${power_of_two}) 
    echo "$output"
    echo "$output" | tail -n 1 >> times.txt
done 
# cat times.txt
output=$(python3 average.py)
echo "$output"
echo "$output" >> speedup.txt
> times.txt
echo ""
echo "bitonic_omp:"
for i in {1..4} 
do
    output=$(./bitonic_omp ${power_of_two})
    echo "$output"
    echo "$output" | tail -n 1 >> times.txt
done  
# cat times.txt
output=$(python3 average.py)
echo "$output"
echo "$output" >> speedup.txt
> times.txt

echo ""
echo "bitonic_mpi:"
for i in {1..4} 
do
    output=$(mpirun -np "${processes}" ./bitonic_mpi ${power_of_two})
    echo "$output"
    echo "$output" | tail -n 1 >> times.txt
done 
# cat times.txt
output=$(python3 average.py)
echo "$output"
echo "$output" >> speedup.txt
> times.txt

echo ""
echo "========================"

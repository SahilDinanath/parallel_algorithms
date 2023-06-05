#!/bin/bash

# Options for running files
file_name="input_2_26.txt"
threads=2
processes=4

make clean
make all 



echo "========================"
echo "Results of Bitonic Sort Implementations"
echo "========================"
echo ""
# Extract the value of num from the file_name
power_of_two=$(echo "$file_name" | sed -n 's/input_2_\([0-9]*\)\.txt/\1/p')

if (( power_of_two <= 22 )); then
  python3 randomarrays.py
else
  echo "Reading from pre-generated file"
fi
> times.txt
> speedup.txt
echo ""
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
echo ""
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

echo ""
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

echo ""
echo "========================"
echo "Average Speedups"
echo "========================"

# Calculate speedup between the first average time and the other two times
python3 speedup.py
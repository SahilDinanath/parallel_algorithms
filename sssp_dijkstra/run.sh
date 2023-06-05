#!/bin/bash
#correctness assertion for serial can be tested with graph_0.txt
#options 
file_name="graph_0.txt"
threads=8
processes=8

make clean
make all
cd input_graphs
cp "$file_name" ../
cd ../

# Serial implementation
echo "========================"
echo "Results of SSSP Dijkstra Implementations"
echo "========================"
echo ""
echo "sssp:"
> times.txt
> speedup.txt
# Run sssp four times and append the last line of output to times.txt
for i in {1..4}
do
    output=$(./sssp ${file_name})
    echo "$output"
    echo "$output" | tail -n 1 >> times.txt
done
out=$(python3 average.py)
echo "$out"
echo "$out" >> speedup.txt
> times.txt

echo ""
echo "sssp_omp:"

# Run sssp_omp four times and append the last line of output to times.txt
for i in {1..4}
do
    output=$(./sssp_omp ${file_name} ${threads})
    echo "$output"
    echo "$output" | tail -n 1 >> times.txt
done
out=$(python3 average.py)
echo "$out"
echo "$out" >> speedup.txt
> times.txt

echo ""
echo "sssp_mpi:"

# Run sssp_mpi four times and append the last line of output to times.txt
for i in {1..4}
do
    output=$(mpirun -np ${processes} ./sssp_mpi ${file_name})
    echo "$output"
    echo "$output" | tail -n 1 >> times.txt
done
out=$(python3 average.py)
echo "$out"
echo "$out" >> speedup.txt
> times.txt

echo ""
echo "========================"
echo "Average Speedups"
echo "========================"
python3 speedup.py

rm ${file_name}

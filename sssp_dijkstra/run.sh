#!/bin/bash

file_name="graph_6.txt"
threads=2

make clean
make all
cd input_graphs
cp "$file_name" ../
cd ../

# Serial implementation
echo "========================"
echo "Results of SSSP Dijkstra Implementations"
echo "========================"

echo "sssp:"
./sssp ${file_name}

# Execute SSSP four times and pipe the last line of output to times.txt
for i in {1..4}
do
    ./sssp "${file_name}" | tail -n 1 >> times.txt
done

# Run Python script to calculate average of execution times
python3 average.py
# cat times.txt
> times.txt
rm "$file_name"
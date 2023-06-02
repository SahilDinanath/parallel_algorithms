#!/bin/bash
file_name="graph_0.txt"
make clean
make all
cd input_graphs
cp $file_name ../
cd ../
# Serial implementation
echo "========================"
echo "Results of SSSP Dijkstra Implementations"
echo "========================"
echo "sssp:"
./sssp ${file_name}
rm $file_name
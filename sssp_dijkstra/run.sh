#!/bin/bash
file_name="graph_10.txt"
threads=2

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
echo ""
echo "sssp_omp:"
./sssp_omp ${file_name} ${threads}
echo "========================"
rm $file_name

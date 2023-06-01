#!/bin/bash
#options for running files
file_name=input_2_3.txt
threads=4
processes=4

make clean
make all 



echo "========================"
echo "Results of Scan Implementations"
echo "========================"
echo "scan:"
#the parameters that scan take are ./scan <file_name> 
./scan ${file_name}
echo ""
echo "scan_omp:"
#the parameters that scan_omp take are ./scan_omp <file_name> <threads>
./scan_omp ${file_name} ${threads}
echo ""
echo "scan_mpi:"
mpirun -np ${processes} ./scan_mpi ${file_name} 
echo ""
echo "========================"

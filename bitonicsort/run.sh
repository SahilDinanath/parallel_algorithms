#!/bin/bash
#options for running files
file_name=input_2_3.txt
threads=4
processes=4

make clean
make all 



echo "========================"
echo "Results of Bitonic Sort Implementations"
echo "========================"
echo "bitonic:"
#the parameters that bitonic take are ./bitonic <file_name> 
./bitonic ${file_name}
echo ""
echo "bitonic_omp:"
#the parameters that bitonic_omp take are ./bitonic_omp <file_name> <threads>
./bitonic_omp ${file_name} ${threads}
echo ""
echo "bitonic_mpi:"
mpirun -np ${processes} ./bitonic_mpi ${file_name} 
echo ""
echo "========================"

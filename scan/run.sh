#!/bin/bash
#options for running files
power_of_two=18
threads=2
processes=2

make clean
make all 

echo "========================"
echo "Results of Scan Implementations"
echo "========================"
echo "scan:"
#the parameters that scan take are ./scan <file_name> 
./scan ${power_of_two}
echo ""
echo "scan_omp:"
#the parameters that scan_omp take are ./scan_omp <file_name> <threads>
./scan_omp ${power_of_two} ${threads}
echo ""
echo "scan_mpi:"
mpirun -np ${processes} ./scan_mpi ${power_of_two} ${processes}
echo ""
echo "========================"

#!/bin/bash
#options for running files
power_of_two=28
threads=8
processes=2

make clean
make all 

echo ${num_of_elements}

echo "========================"
echo "Results of Scan Implementations"
echo "========================"
echo "scan:"
#the parameters that scan take are ./scan <file_name> 
# for i in {1..4}
# do
# ./scan ${power_of_two}
# echo ""
# done
# echo "scan_omp:"
 for i in {1..4}
 do
#the parameters that scan_omp take are ./scan_omp <file_name> <threads>

./scan_omp ${power_of_two} ${threads}
echo ""
done
echo "scan_mpi:"
for i in {1..4}
do
mpirun -np ${processes} ./scan_mpi ${power_of_two} ${processes}
echo ""
done
echo "========================"

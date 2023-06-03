CC = mpicc
CFLAGS= -g -Wall 
LDLIBS = -lm

all: sssp sssp_omp sssp_mpi

sssp: sssp.c
	$(CC) -fopenmp $(CFLAGS) sssp.c -o sssp $(LDLIBS)

sssp_omp: sssp_omp.c
	$(CC) -fopenmp $(CFLAGS) sssp_omp.c -o sssp_omp $(LDLIBS)

sssp_mpi: sssp_mpi.c
	$(CC) $(CFLAGS) sssp_mpi.c -o sssp_mpi $(LDLIBS)

clean:
	rm -f sssp sssp_omp sssp_mpi

CC = mpicc
CFLAGS= -g -Wall -o3
LDLIBS = -lm

all: sssp sssp_omp

sssp: sssp.c
	$(CC) -fopenmp $(CFLAGS) sssp.c -o sssp $(LDLIBS)

sssp_omp: sssp_omp.c
	$(CC) -fopenmp $(CFLAGS) sssp_omp.c -o sssp_omp $(LDLIBS)


clean:
	rm -f sssp sssp_omp

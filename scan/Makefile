CC=mpicc
CFLAGS= -g -Wall -o3
LDLIBS = -lm

SRCS = scan.c scan_omp.c

all: scan scan_omp scan_mpi

scan: scan.c
	$(CC) -fopenmp $(CFLAGS) scan.c -o scan $(LDLIBS)

scan_omp: scan_omp.c
	$(CC) -fopenmp $(CFLAGS) scan_omp.c -o scan_omp $(LDLIBS)

scan_mpi: scan_mpi.c
	$(CC) $(CFLAGS) scan_mpi.c -o scan_mpi $(LDLIBS)

clean:
	rm -f scan scan_omp scan_mpi

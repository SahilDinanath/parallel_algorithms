CC = mpicc
CFLAGS = -std=c99

all: bitonic bitonic_omp bitonic_mpi

bitonic: bitonic.c
	$(CC) $(CFLAGS) -o bitonic bitonic.c

bitonic_omp: bitonic_omp.c
	$(CC) $(CFLAGS) -fopenmp -o bitonic_omp bitonic_omp.c

bitonic_mpi: bitonic_mpi.c
	$(CC) $(CFLAGS) -o bitonic_mpi bitonic_mpi.c

clean:
	rm -f bitonic bitonic_omp bitonic_mpi

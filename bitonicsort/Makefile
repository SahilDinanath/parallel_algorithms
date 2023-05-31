CC = mpicc
CFLAGS = -fopenmp
LDFLAGS = -lm -fopenmp

all: bitonic bitonic_omp bitonic_mpi

bitonic: bitonic.c
	$(CC) $(CFLAGS) -o bitonic bitonic.c

bitonic_omp: bitonic_omp.c
	$(CC) $(CFLAGS) -o bitonic_omp bitonic_omp.c

bitonic_mpi: bitonic_mpi.o
	$(CC) $(CFLAGS) -o bitonic_mpi bitonic_mpi.o $(LDFLAGS)

bitonic_mpi.o: bitonic_mpi.c
	$(CC) $(CFLAGS) -c bitonic_mpi.c

clean:
	rm -f bitonic bitonic_omp bitonic_mpi bitonic_mpi.o

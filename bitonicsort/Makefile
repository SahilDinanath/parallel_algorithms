CC = mpicc
CFLAGS = -fopenmp
LDLIBS = -lm 

all: bitonic bitonic_omp bitonic_mpi

bitonic: bitonic.c
	$(CC) $(CFLAGS) -o bitonic bitonic.c $(LDLIBS) 

bitonic_omp: bitonic_omp.c
	$(CC) $(CFLAGS) -o bitonic_omp bitonic_omp.c $(LDLIBS)

bitonic_mpi: bitonic_mpi.c
	$(CC) $(CFLAGS) -o bitonic_mpi bitonic_mpi.c $(LDLIBS)

clean:
	rm -f bitonic bitonic_omp bitonic_mpi 

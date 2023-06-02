CC = mpicc
CFLAGS = -fopenmp

all: sssp

sssp: sssp.c
	${CC} ${CFLAGS} -o sssp sssp.c
    
clean:
	rm -f sssp
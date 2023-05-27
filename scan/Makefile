CC=gcc
CFLAGS= -fopenmp -g -Wall
LDLIBS = -lm

SRCS = scan.c scan_omp.c

all: scan scan_omp

scan: scan.c
	$(CC) $(CFLAGS) scan.c -o scan $(LDLIBS)

scan_omp: scan.c
	$(CC) $(CFLAGS) scan_omp.c -o scan_omp $(LDLIBS)


clean:
	rm -f scan scan_omp 

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void compAndSwap(int* a, int i, int j, int dir) {
    if ((a[i] > a[j] && dir == 1) || (a[i] < a[j] && dir == 0)) {
        int temp = a[i];
        a[i] = a[j];
        a[j] = temp;
    }
}

void bitonicMerge(int* a, int low, int count, int dir, MPI_Comm comm) {
    if (count > 1) {
        int k = count / 2;
        for (int i = low; i < low + k; i++)
            compAndSwap(a, i, i + k, dir);
        
        int world_rank, size;
        MPI_Comm_rank(comm, &world_rank);
        MPI_Comm_size(comm, &size);
        
        if (world_rank < size / 2) {
            bitonicMerge(a, low, k, dir, comm);
        } else {
            bitonicMerge(a, low + k, k, dir, comm);
        }
    }
}

void bitonicSort(int* a, int low, int count, int dir, MPI_Comm comm) {
    if (count > 1) {
        int k = count / 2;
        
        bitonicSort(a, low, k, 1, comm);
        bitonicSort(a, low + k, k, 0, comm);
        
        bitonicMerge(a, low, count, dir, comm);
    }
}

int main(int argc, char** argv) {
   MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    FILE* file = fopen("arrays.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        MPI_Finalize();
        return 1;
    }

    int a[MAX_SIZE];
    int count = 0;
    int num;

    while (fscanf(file, "%d", &num) == 1) {
        a[count] = num;
        count++;
    }

    fclose(file);

    // Calculate the chunk size for each process
    int chunk_size = count / world_size;

    // Distribute the data among processes
    int* local_a = (int*)malloc(chunk_size * sizeof(int));
    MPI_Scatter(a, chunk_size, MPI_INT, local_a, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Process %d received %d elements.\n", world_rank, chunk_size);

    // Perform bitonic sort on the local data
    bitonicSort(local_a, 0, chunk_size, 1);

    // Gather the sorted data from all processes
    MPI_Gather(local_a, chunk_size, MPI_INT, a, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        printf("Sorted array: ");
        for (int i = 0; i < count; i++) {
            printf("%d ", a[i]);
        }
        printf("\n");
    }

    free(local_a);

    MPI_Finalize(); 
    
    return 0;
}

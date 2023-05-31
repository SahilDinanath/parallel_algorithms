#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

void compAndSwap(long *a, int i, int j, int dir)
{
  if ((a[i] > a[j] && dir == 1) || (a[i] < a[j] && dir == 0))
  {
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
  }
}

void bitonicMerge(long *a, int low, int count, int dir, MPI_Comm comm)
{
  if (count > 1)
  {
    int k = count / 2;
    for (int i = low; i < low + k; i++)
      compAndSwap(a, i, i + k, dir);

    int world_rank, size;
    MPI_Comm_rank(comm, &world_rank);
    MPI_Comm_size(comm, &size);

    if (world_rank < size / 2)
    {
      bitonicMerge(a, low, k, dir, comm);
    }
    else
    {
      bitonicMerge(a, low + k, k, dir, comm);
    }
  }
}

void bitonicSort(long *a, int low, int count, int dir, MPI_Comm comm)
{
  if (count > 1)
  {
    int k = count / 2;

    bitonicSort(a, low, k, 1, comm);
    bitonicSort(a, low + k, k, 0, comm);

    bitonicMerge(a, low, count, dir, comm);
  }
}

void printArray(long input[], long startIndex, long endIndex)
{
  for (long i = startIndex; i < endIndex; i++)
  {
    printf("%ld ", input[i]);
  }
}
void printCharArray(char input[], long startIndex, long endIndex)
{
  for (long i = startIndex; i < endIndex; i++)
  {
    printf("%c ", input[i]);
  }
}
void getFileSize(long *size, FILE *fileName)
{
  fseek(fileName, 0, SEEK_END);
  *size = ftell(fileName);
  fseek(fileName, 0, SEEK_SET);
}
void readFile(char *line, long size, FILE *fileName)
{
  fgets(line, size, fileName);
  fclose(fileName);
}
void convertCharToIntArray(char *fileCharacters, long *input, long size)
{
  for (long i = 0; i < size; i++)
  {
    input[i] = fileCharacters[i] - '0';
  }
}

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);

  int world_rank, world_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // reads in arguments, initializes variables, sets up necessary details for the rest of the program
  char *inputFile = argv[1];
  long inputFileSize;
  long inputArraySize;
  FILE *file = fopen(inputFile, "rb");
  getFileSize(&inputFileSize, file);
  inputFileSize++;
  inputArraySize = inputFileSize - 1;

  // Calculate the chunk size for each process
  long chunk_size = inputArraySize / world_size;

  char *inputFromFile = (char *)malloc(inputArraySize * sizeof(char));
  // using world_size instead of 'sizeof(long)'
  long *input = (long *)malloc(inputArraySize * world_size);
  readFile(inputFromFile, inputFileSize, file);
  convertCharToIntArray(inputFromFile, input, inputArraySize);


  // Distribute the data among processes
  int* local_a = (int*)malloc(chunk_size * sizeof(int));
  MPI_Scatter(input, chunk_size, MPI_INT, local_a, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

  printf("Process %d received %ld elements.\n", world_rank, chunk_size);

  // start timing code here
  double timeStart = omp_get_wtime();

  // Perform bitonic sort on the local data
  bitonicSort(local_a, 0, chunk_size, 1, MPI_COMM_WORLD);
  // stop timing code here
  double timeStop = omp_get_wtime();
  double timeTaken = timeStop - timeStart;

  // Gather the sorted data from all processes
  MPI_Gather(local_a, chunk_size, MPI_INT, input, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

  if (world_rank == 0)
  {
    printArray(input, 0, inputArraySize);
    printf("\n");
  }
  // print out time taken
  //   printf("%f",timeTaken);

  MPI_Finalize();

  return 0;
}

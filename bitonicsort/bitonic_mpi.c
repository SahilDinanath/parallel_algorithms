#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include <omp.h>

void compAndSwap(long *a, int i, int j, int dir)
{
  if ((a[i] > a[j] && dir == 1) || (a[i] < a[j] && dir == 0))
  {
    long temp = a[i];
    a[i] = a[j];
    a[j] = temp;
  }
}

void bitonicMerge(long *a, int low, int count, int dir)
{
  if (count > 1)
  {
    int k = count / 2;

    // Perform bitonic merge
    for (int i = low; i < low + k; i++)
      compAndSwap(a, i, i + k, dir);

    // Recursively perform bitonic merge on both halves
    bitonicMerge(a, low, k, dir);
    bitonicMerge(a, low + k, k, dir);
  }
}


void bitonicSort(long *a, int low, int count, int dir)
{
  if (count > 1)
  {
    int k = count / 2;

    bitonicSort(a, low, k, 1);
    bitonicSort(a, low + k, k, 0);

    bitonicMerge(a, low, count, dir);
  }
}

void printArray(long input[], long startIndex, long endIndex)
{
  for (long i = startIndex; i < endIndex; i++)
  {
    printf("%ld ", input[i]);
  }
  printf("\n");
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

int compareArrays(long *input, long *original, long size) {
  for (int i = 0; i < size; i++) {
    if (input[i] != original[i]) {
      return 0;
    }
  }
  return 1;
}

void iterativeBitonicSort(long* a, int size) {
  int i, j, k;

  for (k = 2; k <= size; k = 2 * k) {
    for (j = k >> 1; j > 0; j = j >> 1) {
      for (i = 0; i < size; i++) {
        int ij = i ^ j;
        if (ij > i) {
          if ((i & k) == 0 && a[i] > a[ij])
            compAndSwap(a, i, ij, 1);
          if ((i & k) != 0 && a[i] < a[ij])
            compAndSwap(a, i, ij, 0);
        }
      }
    }
  }
}

void correctnessAssertion(long *input, long *original, long size) {
  iterativeBitonicSort(original, size);
  int result = compareArrays(input, original, size);
  if (result == 1) {
    printf("Results are correct\n");
  } else {
    printf("Results are incorrect\n");
  }
}

void arrayCopy(long *source, long *destination, long size) {
  for (long i = 0; i < size; i++) {
    destination[i] = source[i];
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

  char *inputFromFile = (char*)malloc(inputArraySize*sizeof(char));

  long *input = (long*)malloc(inputArraySize*sizeof(long));
  long *original = (long *)malloc(inputArraySize * sizeof(long));
  readFile(inputFromFile, inputFileSize, file);
  convertCharToIntArray(inputFromFile, input, inputArraySize);
  arrayCopy(input, original, inputArraySize);

  // Distribute the data among processes
  long* local_a = (long*)malloc(chunk_size * sizeof(long));
  MPI_Scatter(input, chunk_size, MPI_LONG, local_a, chunk_size, MPI_LONG, 0, MPI_COMM_WORLD);

  // printf("Process %d received %ld elements.\n", world_rank, chunk_size);

  // start timing code here
  double timeStart = omp_get_wtime();

  // Perform bitonic sort on the local data
  bitonicSort(local_a, 0, chunk_size, 1);

  // Gather the sorted data from all processes
  MPI_Gather(local_a, chunk_size, MPI_LONG, input, chunk_size, MPI_LONG, 0, MPI_COMM_WORLD);

  if (world_rank == 0)
  {
    // Perform the final bitonic merge on the gathered data
    bitonicSort(input, 0, inputArraySize, 1);
    // stop timing code here
    double timeStop = omp_get_wtime();
    double timeTaken = timeStop - timeStart;
    correctnessAssertion(input, original, inputArraySize);
    // printArray(input, 0, inputArraySize);
    // print out time taken
    printf("%f",timeTaken);
    printf("\n");
  }
  // Free allocated memory
  free(local_a);
  free(inputFromFile);
  free(input);

  MPI_Finalize();

  return 0;
}

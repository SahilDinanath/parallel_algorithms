#include <math.h>
#include <mpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>

void printArray(long input[], long startIndex, long endIndex) {
  for (long i = startIndex; i < endIndex; i++) {
    printf("%ld ", input[i]);
  }
}
void readFile(char *line, long size, FILE *fileName) {
  fgets(line, size, fileName);
  fclose(fileName);
}
void getFileSize(long *size, FILE *fileName) {
  fseek(fileName, 0L, SEEK_END);
  *size = ftell(fileName);
  fseek(fileName, 0L, SEEK_SET);
}
void convertCharToIntArray(char *fileCharacters, long *input, long size) {
  for (long i = 0; i < size; i++) {
    input[i] = fileCharacters[i] - '0';
  }
}

void upSweep(long input[], long size) {
  long previous, next;
  long treeDepth = ceil(log2(size));
  int increment = 0;
  int previousIncrement = 0;
  for (long i = 0; i < treeDepth; i++) {
    previousIncrement = pow(2, i);
    increment = pow(2, i + 1);
    for (long j = 0; j < size; j += increment) {
      previous = j + previousIncrement - 1;
      next = j + increment - 1;
      input[next] = input[previous] + input[next];
    }
  }
}

void downSweep(long input[], long size) {
  long previous, next, temp;
  input[size - 1] = 0;
  long treeDepth = ceil(log2(size));
  int increment = 0;
  int previousIncrement = 0;
  for (long i = treeDepth - 1; i >= 0; i--) {
    previousIncrement = pow(2, i);
    increment = pow(2, i + 1);
    for (long j = 0; j < size; j += increment) {
      previous = j + previousIncrement - 1;
      next = j + increment - 1;
      temp = input[previous];
      input[previous] = input[next];
      input[next] = temp + input[next];
    }
  }
}
void getProcessSum(long input[], long size, long *processSum, long startIndex,
                   long endIndex) {
  for (int i = startIndex; i < endIndex; i++) {
    *processSum += input[i];
  }
  printf("%ld", *processSum);
}

long getStartIndex(int rank, int size, int numOfProcesses) {
  return floor((rank * size) / (float)numOfProcesses);
}

long getEndIndex(int rank, int size, int numOfProcesses) {
  return floor(((rank + 1) * size) / (float)numOfProcesses);
}

void prefixSum(long *input, long size, int argc, char **argv) {
  // variables to be initialized
  int rank;
  int numOfProcesses;
  long processSum = 0;
  // setting up mpi
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numOfProcesses);
  // start timing code here
  double timeStart = MPI_Wtime();

  // get starting and ending indexes of calculations for each process
  long startIndex = getStartIndex(rank, size, numOfProcesses);
  long endIndex = getEndIndex(rank, size, numOfProcesses);

  // calculate processSum
  getProcessSum(input, size, &processSum, startIndex, endIndex);

  // calculate prefix sum
  upSweep(input, size);
  downSweep(input, size);

  // stop timing code here
  double timeStop = MPI_Wtime();
  double timeTaken = timeStop - timeStart;

  // print out time taken
  if (rank == 0) {
    // printf("%f", timeTaken);
    //  printArray(input, 0, inputArraySize);
  }
  MPI_Finalize();
}

int main(int argc, char *argv[]) {
  // reads in arguments, initializes variables, sets up necessary details for
  // the rest of the program
  char *inputFile = argv[1];
  long inputFileSize;
  long inputArraySize;
  FILE *file = fopen(inputFile, "r");
  getFileSize(&inputFileSize, file);
  // reason why it's -1 is because reads in terminating character '\0' which is
  // not apart of the sequence
  inputArraySize = inputFileSize - 1;

  char *inputFromFile = (char *)malloc(inputArraySize * sizeof(char));
  long *input = (long *)malloc(inputArraySize * sizeof(long));

  readFile(inputFromFile, inputFileSize, file);
  convertCharToIntArray(inputFromFile, input, inputArraySize);

  // put algorithm here
  prefixSum(input, inputArraySize, argc, argv);

  return 0;
}

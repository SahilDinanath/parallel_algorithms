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

void upSweep(long input[], long chunkSize, long startIndex, long endIndex) {
  long previous, next;
  long treeDepth = ceil(log2(chunkSize));
  int increment = 0;
  int previousIncrement = 0;
  for (long i = 0; i < treeDepth; i++) {
    previousIncrement = pow(2, i);
    increment = pow(2, i + 1);
    for (long j = startIndex; j < endIndex; j += increment) {
      previous = j + previousIncrement - 1;
      next = j + increment - 1;
      input[next] = input[previous] + input[next];
    }
  }
}

void downSweep(long input[], long chunkSize, long startIndex, long endIndex) {
  long previous, next, temp;
  input[endIndex - 1] = 0;
  long treeDepth = ceil(log2(chunkSize));
  int increment = 0;
  int previousIncrement = 0;
  for (long i = treeDepth - 1; i >= 0; i--) {
    previousIncrement = pow(2, i);
    increment = pow(2, i + 1);
    for (long j = startIndex; j < endIndex; j += increment) {
      previous = j + previousIncrement - 1;
      next = j + increment - 1;
      temp = input[previous];
      input[previous] = input[next];
      input[next] = temp + input[next];
    }
  }
}

void getProcessSum(long input[], long size, long *processSum, long chunkSize,
                   int rank) {
  if (rank == 0) {
    processSum = 0;
    return;
  }
  for (int i = (rank - 1) * chunkSize; i < rank * chunkSize; i++) {
    *processSum += input[i];
  }
  // printf("%ld",*processSum);
}
void applyOffset(long input[], long startIndex, long endIndex, long *processSum,
                 int rank) {
  long sum = 0;
  for (int j = 0; j < rank + 1; j++) {
    sum += processSum[j];
  }
  for (int i = startIndex; i < endIndex; i++) {
    input[i] += sum;
  }
}
long getStartIndex(int rank, long size, int numOfProcesses) {
  return floor((rank * size) / (float)numOfProcesses);
}

long getEndIndex(int rank, long size, int numOfProcesses) {
  return floor(((rank + 1) * size) / (float)numOfProcesses);
}
long getChunkSize(long size, int numOfProcesses) {
  return size / numOfProcesses;
}
void shiftArrayToLeft(long *input, long startIndex, long endIndex) {
  for (long i = startIndex + 1; i < endIndex; i++) {
    input[i - 1] = input[i];
  }
}
void getLastElementPrefixSum(long *input, long endIndex,
                             long initalLastElement) {
  input[endIndex - 1] += initalLastElement;
}

void prefixSum(long *input, long size, int argc, char **argv) {
  // variables to be initialized
  int rank;
  int numOfProcesses;
  long processSum = 0;
  long *inputFinal = (long *)malloc(size * sizeof(long));
  long *processSumArray = (long *)malloc(atoi(argv[2]) * sizeof(long));
  // setting up mpi
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numOfProcesses);
  // start timing code here
  double timeStart = MPI_Wtime();

  // get starting and ending indexes of calculations for each process
  long chunkSize = getChunkSize(size, numOfProcesses);
  long startIndex = getStartIndex(rank, size, numOfProcesses);
  long endIndex = getEndIndex(rank, size, numOfProcesses);
  long endValue = input[endIndex - 1];

  // calculate processSum
  // this is correct
  getProcessSum(input, size, &processSum, chunkSize, rank);
  MPI_Allgather(&processSum, 1, MPI_LONG, processSumArray, 1, MPI_LONG,
                MPI_COMM_WORLD);
  // calculate prefix sum
  upSweep(input, chunkSize, startIndex, endIndex);
  // printArray(input, startIndex, endIndex);
  downSweep(input, chunkSize, startIndex, endIndex);

  shiftArrayToLeft(input, startIndex, endIndex);
  getLastElementPrefixSum(input, endIndex, endValue);
  applyOffset(input, startIndex, endIndex, processSumArray, rank);
  // printf("%ld",processSum);
  // printArray(input , startIndex, endIndex);
  // you thought of basically sending a pointer starting at the end index for
  // each thing and then iterating over chunk elements
  // stop timing code here
  // gathers all the data from each array into one array
  MPI_Gather(&input[startIndex], chunkSize, MPI_LONG, &inputFinal[startIndex],
             chunkSize, MPI_LONG, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  double timeStop = MPI_Wtime();
  double timeTaken = timeStop - timeStart;

  // print out time taken
  if (rank == 0) {
    printf("%f", timeTaken);
    // printArray(processSumArray, 0, numOfProcesses);
    // printArray(inputFinal, 0, size);
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

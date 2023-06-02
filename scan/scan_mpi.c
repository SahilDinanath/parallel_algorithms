#include <math.h>
#include <mpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>
//utility functions
void printArray(long input[], long startIndex, long endIndex) {
  for (long i = startIndex; i < endIndex; i++) {
    printf("%ld ", input[i]);
  }
}
void generateRandomNumbers(long *input, long size) {
  int lower = 0;
  int upper = 9;

  for (int i = 0; i < size; i++) {
    input[i] = (rand() % (upper - lower + 1)) + lower;
  }
}
void serialPrefixSum(long *original, long size) {
  for (long i = 1; i < size; i++) {
    original[i] += original[i - 1];
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

void correctnessAssertion(long *input, long *original, long size) {
  serialPrefixSum(original, size);
  int result = compareArrays(input, original, size);
  if (result == 1) {
    printf("Results are correct");
  } else {
    printf("Results are incorrect");
  }
}
void arrayCopy(long *source, long *destination, long size) {
  for (long i = 0; i < size; i++) {
    destination[i] = source[i];
  }
}

//prefix sum code
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

void prefixSum(long *input, long *original, long size, int argc, char **argv) {
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
  if (rank == 0) {
    generateRandomNumbers(input, size);
    arrayCopy(input, original, size);
  }
  MPI_Bcast(input, size, MPI_LONG, 0, MPI_COMM_WORLD);
  double timeStart = MPI_Wtime();

  // get starting and ending indexes of calculations for each process
  long chunkSize = getChunkSize(size, numOfProcesses);
  long startIndex = getStartIndex(rank, size, numOfProcesses);
  long endIndex = getEndIndex(rank, size, numOfProcesses);
  long endValue = input[endIndex - 1];

  // calculate processSum
  getProcessSum(input, size, &processSum, chunkSize, rank);
  MPI_Allgather(&processSum, 1, MPI_LONG, processSumArray, 1, MPI_LONG,
                MPI_COMM_WORLD);
  // calculate prefix sum
  upSweep(input, chunkSize, startIndex, endIndex);
  downSweep(input, chunkSize, startIndex, endIndex);

  shiftArrayToLeft(input, startIndex, endIndex);
  getLastElementPrefixSum(input, endIndex, endValue);
  applyOffset(input, startIndex, endIndex, processSumArray, rank);
  // you thought of basically sending a pointer starting at the end index for
  // each thing and then iterating over chunk elements
  // gathers all the data from each array into one array
  MPI_Gather(&input[startIndex], chunkSize, MPI_LONG, &inputFinal[startIndex],
             chunkSize, MPI_LONG, 0, MPI_COMM_WORLD);
  // stop timing code here
  double timeStop = MPI_Wtime();
  double timeTaken = timeStop - timeStart;

  // print out time taken
  if (rank == 0) {
    printf("%f\n", timeTaken);
    correctnessAssertion(inputFinal, original, size);
    // printArray(processSumArray, 0, numOfProcesses);
    // printArray(inputFinal, 0, size);
  }
  MPI_Finalize();
}

int main(int argc, char *argv[]) {
  // reads in arguments, initializes variables, sets up necessary details for
  // the rest of the program
  long inputSize = pow(2, atol(argv[1]));
  // reason why it's -1 is because reads in terminating character '\0' which is
  // not apart of the sequence
  long *input = (long *)malloc(inputSize * sizeof(long));
  long *original = (long *)malloc(inputSize * sizeof(long));
  // put algorithm here
  prefixSum(input, original, inputSize, argc, argv);

  return 0;
}

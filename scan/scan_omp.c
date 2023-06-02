#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>


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

void upSweep(long input[], long chunkSize, long startIndex, long endIndex) {
  long previous, next;
  long treeDepth = ceil(log2(chunkSize));
  int increment = 0;
  int previousIncrement = 0;
  for (long i = 0; i < treeDepth; i++) {
    previousIncrement = pow(2, i);
    increment = pow(2, i + 1);
#pragma omp parallel shared(increment, previousIncrement) private(previous,    \
                                                                      next)
    {
#pragma omp for
      for (long j = startIndex; j < endIndex; j += increment) {
        previous = j + previousIncrement - 1;
        next = j + increment - 1;
        input[next] = input[previous] + input[next];
      }
    }
  }
}

void downSweep(long input[], long chunkSize,long startIndex,long endIndex) {
  long previous, next, temp;
  input[endIndex- 1] = 0;
  long treeDepth = ceil(log2(chunkSize));
  int increment = 0;
  int previousIncrement = 0;
  for (long i = treeDepth - 1; i >= 0; i--) {
    previousIncrement = pow(2, i);
    increment = pow(2, i + 1);
#pragma omp parallel shared(increment, previousIncrement) private(             \
        previous, next, temp)
    {
#pragma omp for
      for (long j = startIndex; j < endIndex; j += increment) {
        previous = j + previousIncrement - 1;
        next = j + increment - 1;
        temp = input[previous];
        input[previous] = input[next];
        input[next] = temp + input[next];
      }
    }
  }
}


void shiftArrayToLeft(long *input, long startIndex, long endIndex) {
  startIndex++;
  #pragma omp parallel for shared(startIndex,endIndex)
  for (long i = startIndex; i < endIndex; i++) {
    input[i - 1] = input[i];
  }
}
void getLastElementPrefixSum(long *input, long endIndex,
                             long initalLastElement) {
  input[endIndex - 1] += initalLastElement;
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
void getProcessSum(long input[], long size, long *processSum, long chunkSize,
                   int rank) {
  if (rank == 0) {
    *processSum = 0;
    return;
  }
  int start = (rank - 1) * chunkSize;
  int end = rank * chunkSize;
  
  #pragma omp parallel for shared(start,end)
  for (int i = start; i < end; i++) {
    *processSum += input[i];
  }
}
void applyOffset(long input[], long startIndex, long endIndex, long *processSum,
                 int rank) {
  long sum = 0;
  for (int j = 0; j < rank + 1; j++) {
    sum += processSum[j];
  }
  #pragma omp parallel for shared(startIndex,endIndex)
  for (int i = startIndex; i < endIndex; i++) {
    input[i] += sum;
  }
}

void prefixSum(long *input, long size, int threads) {
  long *processSumArray = (long *)malloc(threads * sizeof(long));

#pragma omp parallel firstprivate(input) shared(processSumArray)
  {
    int numOfProcesses = omp_get_num_threads();
    int rank = omp_get_thread_num();
    long chunkSize = getChunkSize(size, numOfProcesses);
    long startIndex = getStartIndex(rank, size, numOfProcesses);
    long endIndex = getEndIndex(rank, size, numOfProcesses);
    long endValue = input[endIndex - 1];
    long processSum = 0;
    getProcessSum(input, size, &processSum, chunkSize, rank);
    processSumArray[rank] = processSum;  
    #pragma omp barrier
    upSweep(input, chunkSize,startIndex,endIndex);
    downSweep(input, chunkSize,startIndex,endIndex);
    shiftArrayToLeft(input,startIndex,endIndex);
    getLastElementPrefixSum(input,endIndex, endValue);
    applyOffset(input,startIndex,endIndex,processSumArray,rank);
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

int main(int argc, char *argv[]) {
  long inputSize = pow(2, atol(argv[1]));
  int threads = atoi(argv[2]);

  omp_set_dynamic(0);
  omp_set_num_threads(threads);

  long *input = (long *)malloc(inputSize * sizeof(long));
  long *original = (long *)malloc(inputSize * sizeof(long));
  // first generate random numbers
  generateRandomNumbers(input, inputSize);
  // copy it so that the end result can be tested
  arrayCopy(input, original, inputSize);
  // start timing code here
  double timeStart = omp_get_wtime();

  // put algorithm here
  prefixSum(input, inputSize, threads);

  // stop timing code here
  double timeStop = omp_get_wtime();
  double timeTaken = timeStop - timeStart;

  // print out time taken
  printf("%f\n", timeTaken);
  // printArray(input, 0, inputSize);
  correctnessAssertion(input, original, inputSize);
  return 0;
}

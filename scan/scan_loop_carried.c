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
void shiftArrayToLeft(long *input, long startIndex, long endIndex) {
  for (long i = startIndex + 1; i < endIndex; i++) {
    input[i - 1] = input[i];
  }
}
void prefixSum(long *input, long size) {
  for (long i = 1; i < size; i++) {
    input[i] += input[i - 1];
  }
}

int main(int argc, char *argv[]) {
  // reads in arguments, initializes variables, sets up necessary details for
  // the rest of the program
  long inputSize = atol(argv[1]);
  long *input = (long *)malloc(inputSize * sizeof(long));
  generateRandomNumbers(input, inputSize);
  // start timing code here
  double timeStart = omp_get_wtime();

  // put algorithm here
  prefixSum(input, inputSize);

  // stop timing code here
  double timeStop = omp_get_wtime();
  double timeTaken = timeStop - timeStart;

  // print out time taken
  //  printf("%f",timeTaken);
  printArray(input, 0, inputSize);

  printf("\n");
  return 0;
}

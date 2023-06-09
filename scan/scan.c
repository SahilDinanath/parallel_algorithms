#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>


void printArray(long input[], long startIndex, long endIndex) {
  for (long i = startIndex; i < endIndex; i++) {
    printf("%ld ", input[i]);
  }
  printf("\n");
}
void generateRandomNumbers(long *input, long size) {
  int lower = 0;
  int upper = 9;

  for (int i = 0; i < size; i++) {
    input[i] = (rand() % (upper - lower + 1)) + lower;
  }
}

void upSweep(long input[], long size) {
  long previous, next;
  long treeDepth = ceil(log2(size));
  long increment = 0;
  long previousIncrement = 0;
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
  long increment = 0;
  long previousIncrement = 0;
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

void shiftArrayToLeft(long *input, long size) {
  for (long i = 1; i < size; i++) {
    input[i - 1] = input[i];
  }
}
void getLastElementPrefixSum(long *input, long size, long initalLastElement) {
  input[size - 1] += initalLastElement;
}

void prefixSum(long *input, long size) {
  long tempLastElement = input[size - 1];
  upSweep(input, size);
  downSweep(input, size);
  shiftArrayToLeft(input, size);
  getLastElementPrefixSum(input, size, tempLastElement);
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
    printf("Results are correct:\n");
  } else {
    printf("Results are incorrect:\n");
  }
}

void arrayCopy(long *source, long *destination, long size) {
  for (long i = 0; i < size; i++) {
    destination[i] = source[i];
  }
}
int main(int argc, char *argv[]) {
  // reads in arguments, initializes variables, sets up necessary details for
  // the rest of the program
  long inputSize = pow(2, atol(argv[1]));

  long *input = (long *)malloc(inputSize * sizeof(long));
  long *original = (long *)malloc(inputSize * sizeof(long));

  generateRandomNumbers(input, inputSize);
  arrayCopy(input, original, inputSize);

  // start timing code here
  double timeStart = omp_get_wtime();

  // put algorithm here
  prefixSum(input, inputSize);

  // stop timing code here
  double timeStop = omp_get_wtime();
  double timeTaken = timeStop - timeStart;

  // do correctnessAssertion here
  correctnessAssertion(input, original, inputSize);
  // print out time taken
  // printArray(input, 0, inputSize);
  printf("%f\n", timeTaken);
  return 0;
}

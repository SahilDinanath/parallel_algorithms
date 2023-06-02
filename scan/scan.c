#include <math.h>
#include <omp.h>
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

void serialPrefixSum(long *input, long size) {
  for (long i = 1; i < size; i++) {
    input[i] += input[i - 1];
  }
  printArray(input,0,size);
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
  // reads in arguments, initializes variables, sets up necessary details for
  // the rest of the program
  char *inputFile = argv[1];
  long inputFileSize;
  long inputArraySize;
  FILE *file = fopen(inputFile, "r");
  getFileSize(&inputFileSize, file);
  inputArraySize = inputFileSize - 1;

  char *inputFromFile = (char *)malloc(inputArraySize * sizeof(char));
  long *input = (long *)malloc(inputArraySize * sizeof(long));
  long *original = (long *)malloc(inputArraySize * sizeof(long));

  readFile(inputFromFile, inputFileSize, file);
  convertCharToIntArray(inputFromFile, input, inputArraySize);
  arrayCopy(input, original, inputArraySize);
  // start timing code here
  double timeStart = omp_get_wtime();

  // put algorithm here
  prefixSum(input, inputArraySize);

  // stop timing code here
  double timeStop = omp_get_wtime();
  double timeTaken = timeStop - timeStart;

  // print out time taken
  printf("%f\n", timeTaken);
  // do correctnessAssertion here
  correctnessAssertion(input, original, inputArraySize);
  printArray(input, 0, inputArraySize);
  return 0;
}

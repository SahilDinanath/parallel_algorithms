#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
//power should correspond to the text file
#define SIZE pow(2, 16)

void printArray(long input[], long startIndex, long endIndex) {
  for (long i = startIndex; i < endIndex; i++) {
    printf("%ld ", input[i]);
  }
}
void readFile(char *line, long size, char *fileName) {
  FILE *file = fopen(fileName, "r");

  fgets(line, size, file);
  fclose(file);
}
void convertCharToIntArray(char *fileCharacters, long *input, long size) {
  for (long i = 0; i < size; i++) {
    input[i] = fileCharacters[i] - '0';
  }
}

void prefixSum(long *input, long size) {
  for (long i = 1; i < size; i++) {
    input[i] += input[i - 1];
  }
}
int main(int argc, char *argv[]) {
  //input size of file is one larger then the actual input as the last value in the array of reading in any string is '\0'  

  char *inputFile = argv[1];
  long inputSize = (long)SIZE;

  long inputSizeOfTextFile = inputSize+1;
  char fileInput[inputSizeOfTextFile];
  long input[inputSize];

  readFile(fileInput, inputSizeOfTextFile, inputFile);
  convertCharToIntArray(fileInput, input, inputSize);

  //start timing code here
  double timeStart = omp_get_wtime();

  //put algorithm here
  prefixSum(input, inputSize);

  //stop timing code here
  double timeStop = omp_get_wtime();
  double timeTaken = timeStop - timeStart;
  // printf("%f",timeTaken);
  printArray(input, 0, inputSize);

  printf("\n");
  return 0;
}

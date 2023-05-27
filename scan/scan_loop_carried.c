#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE pow(2, 16)

void printArray(int input[], int startIndex, int endIndex) {
  for (int i = startIndex; i < endIndex; i++) {
    printf("%d ", input[i]);
  }
}
void readFile(char *line, int size, char *fileName) {
  FILE *file = fopen(fileName, "r");

  fgets(line, size, file);
  fclose(file);
}
void convertCharToIntArray(char *fileCharacters, int *input, int size) {
  for (int i = 0; i < size; i++) {
    input[i] = fileCharacters[i] - '0';
  }
}

void prefixSum(int *input, int size) {
  for (int i = 1; i < size; i++) {
    input[i] += input[i - 1];
  }
}
int main(int argc, char *argv[]) {
  //input size of file is one larger then the actual input as the last value in the array of reading in any string is '\0'  
  int inputSize = (int)SIZE;
  int inputSizeOfTextFile = inputSize+1;

  char fileInput[inputSizeOfTextFile];
  int input[inputSize];

  readFile(fileInput, inputSizeOfTextFile, "input_2_16.txt");

  convertCharToIntArray(fileInput, input, inputSize);

  prefixSum(input, inputSize);

  printArray(input, 0, inputSize);
  return 0;
}

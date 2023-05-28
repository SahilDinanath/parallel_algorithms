#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
//power should correspond to the text file

void printArray(long input[], long startIndex, long endIndex) {
  for (long i = startIndex; i < endIndex; i++) {
    printf("%ld ", input[i]);
  }
}
void getFileSize(long *size,FILE *fileName){
  fseek(fileName,0L,SEEK_END);
  *size = ftell(fileName);
  fseek(fileName,0L,SEEK_SET);
}
void readFile(char *line, long size, FILE *fileName) {
  fgets(line, size, fileName);
  fclose(fileName);
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
  //reads in arguments, initializes variables, sets up necessary details for the rest of the program
  char *inputFile = argv[1];
  long inputFileSize;
  long inputArraySize;
  FILE *file = fopen(inputFile, "r");
  getFileSize(&inputFileSize, file);
  inputArraySize = inputFileSize-1;

  char *inputFromFile = (char*)malloc(inputArraySize*sizeof(char));
  long *input = (long*)malloc(inputArraySize*sizeof(long));

  readFile(inputFromFile, inputFileSize, file);
  convertCharToIntArray(inputFromFile, input, inputArraySize);

  //start timing code here
  double timeStart = omp_get_wtime();

  //put algorithm here
  prefixSum(input, inputArraySize);

  //stop timing code here
  double timeStop = omp_get_wtime();
  double timeTaken = timeStop - timeStart;


  //print out time taken
  // printf("%f",timeTaken);
  printArray(input, 0, inputArraySize);

  printf("\n");
  return 0;
}

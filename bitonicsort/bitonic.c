#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void compAndSwap(long* a, int i, int j, int dir) {
    if ((a[i] > a[j] && dir == 1) || (a[i] < a[j] && dir == 0)) {
        int temp = a[i];
        a[i] = a[j];
        a[j] = temp;
    }
}

void bitonicMerge(long* a, int low, int count, int dir) {
    if (count > 1) {
        int k = count / 2;
        for (int i = low; i < low + k; i++)
            compAndSwap(a, i, i + k, dir);
        
        bitonicMerge(a, low, k, dir);
        bitonicMerge(a, low + k, k, dir);
    }
}

void bitonicSort(long* a, int low, int count, int dir) {
    if (count > 1) {
        int k = count / 2;
        
        bitonicSort(a, low, k, 1);
        bitonicSort(a, low + k, k, 0);
        
        bitonicMerge(a, low, count, dir);
    }
}

void printArray(long input[], long startIndex, long endIndex) {
  for (long i = startIndex; i < endIndex; i++) {
    printf("%ld ", input[i]);
  }
}
void printCharArray(char input[], long startIndex, long endIndex) {
  for (long i = startIndex; i < endIndex; i++) {
    printf("%c ", input[i]);
  }
}
void getFileSize(long *size,FILE *fileName){
  fseek(fileName,0,SEEK_END);
  *size = ftell(fileName);
  fseek(fileName,0,SEEK_SET);
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



int main(int argc, char *argv[]) {
  //reads in arguments, initializes variables, sets up necessary details for the rest of the program
  char *inputFile = argv[1];
  long inputFileSize;
  long inputArraySize;
  FILE *file = fopen(inputFile, "rb");
  getFileSize(&inputFileSize, file);
  inputFileSize++;
  inputArraySize = inputFileSize-1;

  char *inputFromFile = (char*)malloc(inputArraySize*sizeof(char));
  long *input = (long*)malloc(inputArraySize*sizeof(long));
  readFile(inputFromFile, inputFileSize, file);
  convertCharToIntArray(inputFromFile, input, inputArraySize);
  //start timing code here
  double timeStart = omp_get_wtime();

  //put algorithm here
 bitonicSort(input, 0, inputArraySize, 1);

  //stop timing code here
  double timeStop = omp_get_wtime();
  double timeTaken = timeStop - timeStart;


  //print out time taken
//   printf("%f",timeTaken);
  printArray(input, 0, inputArraySize);

  printf("\n");
  return 0;
}
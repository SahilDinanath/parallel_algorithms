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
  printf("\n");
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

int compareArrays(long *input, long *original, long size) {
  for (int i = 0; i < size; i++) {
    if (input[i] != original[i]) {
      return 0;
    }
  }
  return 1;
}

void iterativeBitonicSort(long* a, int size) {
  int i, j, k;

  for (k = 2; k <= size; k = 2 * k) {
    for (j = k >> 1; j > 0; j = j >> 1) {
      for (i = 0; i < size; i++) {
        int ij = i ^ j;
        if (ij > i) {
          if ((i & k) == 0 && a[i] > a[ij])
            compAndSwap(a, i, ij, 1);
          if ((i & k) != 0 && a[i] < a[ij])
            compAndSwap(a, i, ij, 0);
        }
      }
    }
  }
}

void correctnessAssertion(long *input, long *original, long size) {
  iterativeBitonicSort(original, size);
  int result = compareArrays(input, original, size);
  if (result == 1) {
    printf("Results are correct\n");
  } else {
    printf("Results are incorrect\n");
  }
}

void arrayCopy(long *source, long *destination, long size) {
  for (long i = 0; i < size; i++) {
    destination[i] = source[i];
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
  long *original = (long *)malloc(inputArraySize * sizeof(long));
  readFile(inputFromFile, inputFileSize, file);
  convertCharToIntArray(inputFromFile, input, inputArraySize);
  arrayCopy(input, original, inputArraySize);
  //start timing code here
  double timeStart = omp_get_wtime();

  //put algorithm here
 bitonicSort(input, 0, inputArraySize, 1);

  //stop timing code here
  double timeStop = omp_get_wtime();
  double timeTaken = timeStop - timeStart;

  correctnessAssertion(input, original, inputArraySize);
  // printArray(input, 0, inputArraySize);
  // printf("\n");
  // print out time taken
  printf("%f",timeTaken);

  printf("\n");
  return 0;
}
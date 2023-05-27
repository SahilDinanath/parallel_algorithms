#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void upSweep(int input[], int size) {
  int previous, next;
  int treeDepth = ceil(log2(size));
  for (int i = 0; i < treeDepth; i++) {
    for (int j = 0; j < size; j += pow(2, i + 1)) {
      previous = j + pow(2, i) - 1;
      next = j + pow(2, i + 1) - 1;
      input[next] = input[previous] + input[next];
    }
  }
}

void downSweep(int input[], int size) {
  int previous, next, temp;
  input[size - 1] = 0;
  int treeDepth = ceil(log2(size));
  for (int i = treeDepth - 1; i >= 0; i--) {
    for (int j = 0; j < size; j += pow(2, i + 1)) {
      previous = j + pow(2, i) - 1;
      next = j + pow(2, i + 1) - 1;
      temp = input[previous];
      input[previous] = input[next];
      input[next] = temp + input[next];
    }
  }
}
void printArray(int input[], int startIndex, int endIndex) {
  for (int i = startIndex; i < endIndex; i++) {
    printf("%d ", input[i]);
  }
}
void calculatePrefixSum(int input[])
double getAverageOfRuns(int input[], int inputSize, int runs) {
  double average = 0;

  for (int i = 0; i < runs; i++) {
    double timer = omp_get_wtime();

    upSweep(input, inputSize);
    downSweep(input, inputSize);

    timer = omp_get_wtime() - timer;
    average += timer;
  }
  return average / runs;
}

int main(int argc, char *argv[]) {
  FILE *file = fopen("input_2_16.txt", "r");
  int *input;
  size_t n = 0;
  int c;

  fseek(file, 0, SEEK_END);
  long f_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  input = (int *)malloc(f_size * sizeof(int));

  while ((c = fgetc(file)) != EOF) {
    input[n++] = (int)c;
  }

  int inputSize = f_size;

  int lastElementOfInput = input[inputSize - 1];
  double average = getAverageOfRuns(input, inputSize, 10);

  printf("time taken: %f\n", average);
  return 0;
}

#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define THREADS 8

void getUpSweep(int input[], int size) {

  int treeDepth = ceil(log2(size));
  int increment;
  for (int i = 0; i < treeDepth; i++) {
    increment = pow(2, i + 1);
#pragma omp parallel
    {
      int previous, next;
#pragma omp for schedule(static, size / THREADS)
      for (int j = 0; j < size; j += increment) {
        previous = j + pow(2, i) - 1;
        next = j + pow(2, i + 1) - 1;
        input[next] = input[previous] + input[next];
      }
    }
  }
}
void getUpSweepUsingProcessorSum(int input[], int size) {

  int chunkSize = size / THREADS;
  int treeDepth = ceil(log2(chunkSize));
#pragma omp parallel
  {
    int processor = omp_get_thread_num();
    int previous, next;
    int increment;
    for (int i = 0; i < treeDepth; i++) {
      increment = pow(2, i + 1);
      for (int j = chunkSize * processor; j < chunkSize * (processor + 1);
           j += increment) {
        previous = j + pow(2, i) - 1;
        next = j + pow(2, i + 1) - 1;
        input[next] = input[previous] + input[next];
      }
    }
  }
}

void getDownSweep(int input[], int size) {
  int treeDepth = ceil(log2(size));
  input[size - 1] = 0;

  for (int i = treeDepth - 1; i >= 0; i--) {
#pragma omp parallel
    {
      int previous, next, temp;
      int increment = pow(2, i + 1);
#pragma omp for schedule(static, size / THREADS)
      for (int j = 0; j < size; j += increment) {
        previous = j + pow(2, i) - 1;
        next = j + pow(2, i + 1) - 1;
        temp = input[previous];
        input[previous] = input[next];
        input[next] = temp + input[next];
      }
    }
  }
}
void generateArrayOfRandomNumbers(int input[], int size) {
  srand(time(NULL));
  for (int i = 0; i < size; i++) {
    input[i] = rand() % 100;
  }
}
void getDownSweepUsingProcessorSum(int input[], int size) {
  input[size - 1] = 0;
  int chunkSize = size / THREADS;
  int treeDepth = ceil(log2(chunkSize));
#pragma omp parallel
  {
    int processor = omp_get_thread_num();

    for (int i = treeDepth - 1; i >= 0; i--) {
      int previous, next, temp;
      int increment = pow(2, i + 1);
      for (int j = processor * chunkSize; j < chunkSize * (processor + 1);
           j += increment) {
        previous = j + pow(2, i) - 1;
        next = j + pow(2, i + 1) - 1;
        temp = input[previous];
        input[previous] = input[next];
        input[next] = temp + input[next];
      }
    }
  }
}

void printArray(int input[], int startIndex, int endIndex) {
  for (int i = startIndex; i < endIndex; i++) {
    printf("%d ", input[i]);
  }
}

void getProcessorSum(int input[], int processorSum[], int size) {
#pragma omp parallel
  {
    int processor = omp_get_thread_num();
    int numOfProcessors = omp_get_num_threads();

    int chunkSize = size / numOfProcessors;

    processorSum[processor] = input[chunkSize * processor];

    for (int i = 1; i < chunkSize; i++) {
      processorSum[processor] =
          processorSum[processor] + input[chunkSize * processor + i];
    }
  }
  getUpSweep(processorSum, THREADS);
  getDownSweep(processorSum, THREADS);
}

void addProcessorSumsToPrescan(int input[], int size, int processorSum[]) {

  int chunkSize = size / THREADS;
#pragma omp parallel
  {
    int processor = omp_get_thread_num();
    for (int j = chunkSize * processor; j < chunkSize * (processor + 1); j++) {
      input[j] = processorSum[processor] + input[j];
    }
  }
}
double averageOfRuns(int input[], int size, int runs) {
  double average = 0;
  for (int i = 0; i < runs; i++) {
    double timer = omp_get_wtime();
    getUpSweep(input, size);
    getDownSweep(input, size);
    timer = omp_get_wtime() - timer;
    average += timer;
  }
  return average / runs;
}
int main(int argc, char *argv[]) {
  omp_set_dynamic(0);
  omp_set_num_threads(THREADS);
  int processorSum[THREADS];

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
  // int input[] = {3, 1, 7, 0, 4, 1, 6, 3};
  // int input[131072 * 8];
  // int inputSize = sizeof(input) / sizeof(int);

  // generateArrayOfRandomNumbers(input, inputSize);
  int lastNum = input[inputSize - 1];

  double timer = averageOfRuns(input, inputSize, 10);

  // getProcessorSum(input, processorSum, inputSize);
  // printArray(processorSum, 0, THREADS);
  // printf("\n");
  // getUpSweep(input, inputSize);
  // getDownSweep(input , inputSize);
  // getUpSweepUsingProcessorSum(input, inputSize);
  // printf("array after adding processr sums to up sweep:\n");
  // printArray(input, 0, inputSize);
  // printf("\n");

  // getDownSweepUsingProcessorSum(input, inputSize);
  // addProcessorSumsToPrescan(input, inputSize, processorSum);

  // printArray(input, 1, inputSize);
  // printf("%d",input[inputSize-1]+lastNum);
  // printf("\n");

  printf("time taken: %f\n", timer);
  return 0;
}

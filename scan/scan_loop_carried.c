#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
void generateArrayOfRandomNumbers(int input[],int size){
  srand(time(NULL));
  for(int i = 0; i < size; i++){
    input[i] = rand()%100;
  }
}
void printArray(int input[],int startIndex,int endIndex){
  for(int i = startIndex; i < endIndex;i++){
    printf("%d ",input[i]);
  }
}

int main(int argc, char *argv[])
{

  while(fgets(line,100,p_file) != NULL)
{
    char *p = strtok(line, ":");
    while(p)
    {
        printf("%s\n", p); //Store into array or variables
        p=strtok(NULL, ":");
    }
}
  int *input = malloc(inputSize*sizeof(int));

  for(int i = 1; i < argc; i++){
    input[i-1] = atoi(argv[i]);
  }
  // int input[] = {3,1,7,0,4,1,6,3};
  // int input[131072*8];
  // int inputSize = sizeof(input)/sizeof(int);
  // generateArrayOfRandomNumbers(input, inputSize);

  
  double timer = omp_get_wtime();
  
  for(int i = 1; i < inputSize; i++){
    input[i] += input[i-1];
  }
  

  timer = omp_get_wtime()-timer;
  printArray(input, 0, inputSize);
  printf("\n");
  printf("time taken: %f\n",timer);
  free(input);
  return 0;
}

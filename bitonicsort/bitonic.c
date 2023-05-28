#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1000000

void compAndSwap(long long* a, int i, int j, int dir) {
    if ((a[i] > a[j] && dir == 1) || (a[i] < a[j] && dir == 0)) {
        int temp = a[i];
        a[i] = a[j];
        a[j] = temp;
    }
}

void bitonicMerge(long long* a, int low, int count, int dir) {
    if (count > 1) {
        int k = count / 2;
        for (int i = low; i < low + k; i++)
            compAndSwap(a, i, i + k, dir);
        
        bitonicMerge(a, low, k, dir);
        bitonicMerge(a, low + k, k, dir);
    }
}

void bitonicSort(long long* a, int low, int count, int dir) {
    if (count > 1) {
        int k = count / 2;
        
        bitonicSort(a, low, k, 1);
        bitonicSort(a, low + k, k, 0);
        
        bitonicMerge(a, low, count, dir);
    }
}

int main() {
   FILE* file = fopen("arrays.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    long long a[MAX_SIZE];
    int count = 0;
    int num;

    while (fscanf(file, "%d", &num) == 1) {
        a[count] = num;
        count++;
    }

    fclose(file);

    printf("Original array: ");
    for (int i = 0; i < count; i++) {
        printf("%lld ", a[i]);
    }
    printf("\n");

    bitonicSort(a, 0, count, 1);

    printf("Sorted array: ");
    for (int i = 0; i < count; i++) {
        printf("%lld ", a[i]);
    }
    printf("\n");
 

    return 0; 
}

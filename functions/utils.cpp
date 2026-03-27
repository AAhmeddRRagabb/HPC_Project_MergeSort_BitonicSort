#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../mainHeader.h"

void printArray(int A[], int size) {
    int i;
    for (i=0; i < size; i++)
        printf("%d ", A[i]);
    printf("\n");
}




void generate_array(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100000;
    }
}




double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}




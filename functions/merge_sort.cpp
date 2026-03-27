#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <string.h>
#include "../mainHeader.h"

#define THRESHOLD 1000

// Merges two subarrays of arr[].
void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    
    int *L = (int*) malloc(n1 * sizeof(int));
    int *R = (int*) malloc(n2 * sizeof(int));

    
    for (i = 0; i < n1; i++) L[i] = arr[l + i];
    for (j = 0; j < n2; j++) R[j] = arr[m + 1+ j];


    // merge
    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }

  
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];


    free(L);
    free(R);
}


void mergeSort(int arr[], int l, int r) {
    if (l < r)
    {
        int m = l+(r-l)/2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);

        merge(arr, l, m, r);
    }
}


void mergeSortParallel(int arr[], int l, int r) {
    if (l < r) {

        if (r - l < THRESHOLD) {
            mergeSort(arr, l, r);  // serial
            return;
        }
        
        int m = l+(r-l)/2;

        #pragma omp task
        mergeSortParallel(arr, l, m);

        #pragma omp task
        mergeSortParallel(arr, m+1, r);

        #pragma omp taskwait

        merge(arr, l, m, r);
    }
}


// measure performance
double measure_merge_sort(int repeats, int arr_size, char* impl) {
    double total_time = 0;

    for (int i = 0; i < repeats; i++) {
        // generate array
        int *arr = new int[arr_size];
        generate_array(arr, arr_size);

        // sort
        double start_time = get_time();

        if (strcmp(impl, "serial") == 0) {
            mergeSort(arr, 0, arr_size - 1);
        } else if (strcmp(impl, "openmp") == 0) {
            #pragma omp parallel
            {
                #pragma omp single
                mergeSortParallel(arr, 0, arr_size - 1);
            }
        }

        double end_time = get_time();

        // accumulate time for avg
        total_time += (end_time - start_time);


        delete[] arr;
    }

    double avg = total_time / repeats;
    return avg;
}





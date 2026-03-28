#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <string.h>
#include "headers.h"
using namespace std;

#define THRESHOLD 1000


void merge(int arr[], int l, int m, int r, int temp[]) {
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i <= m) temp[k++] = arr[i++];
    while (j <= r) temp[k++] = arr[j++];
    
    
    for (i = l; i <= r; i++) arr[i] = temp[i];
}


void mergeSort(int arr[], int l, int r, int temp[]) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m, temp); 
        mergeSort(arr, m + 1, r, temp); 
        merge(arr, l, m, r, temp);
    }
}


void mergeSortParallel(int arr[], int l, int r, int temp[]) {
    if (l < r) {

        // cut-off to avoid exhausted resources
        if (r - l < THRESHOLD) {
            mergeSort(arr, l, r, temp);  
            return;
        }
        
        int m = l + (r - l) / 2;

        #pragma omp task
        mergeSortParallel(arr, l, m, temp);

        #pragma omp task
        mergeSortParallel(arr, m + 1, r, temp);

        #pragma omp taskwait

        merge(arr, l, m, r, temp);
    }
}


// measure performance
double measure_merge_sort(int repeats, int threads, int arr_size, char* impl, char* array_distribution, int seed) {
    double total_time = 0;

    for (int i = 0; i < repeats; i++) {
        srand(seed);

        // generate array
        int *arr = new int[arr_size];
        int *temp_buffer = new int[arr_size];

        if      (strcmp(array_distribution, "uniform") == 0)       generate_uniform(arr, arr_size);
        else if (strcmp(array_distribution, "gaussian") == 0)      generate_gaussian(arr, arr_size);
        else if (strcmp(array_distribution, "nearly_sorted") == 0) generate_nearly_sorted(arr, arr_size);
        else if (strcmp(array_distribution, "reversed") == 0)      generate_reversed(arr, arr_size);
        else {
            cout << "Invalid Array Distribution" << endl;
            return -1.0;
        }

        // sort
        double start_time = get_time();

        if (strcmp(impl, "serial") == 0) {
            mergeSort(arr, 0, arr_size - 1, temp_buffer);
        } 
        
        else if (strcmp(impl, "omp") == 0) {
            omp_set_num_threads(threads);
            #pragma omp parallel
            {
                #pragma omp single
                mergeSortParallel(arr, 0, arr_size - 1, temp_buffer);
            }
        } 
        
        else {
            printf("Invalid Implementation Method");
            return -1;
        }

        
        double end_time = get_time();

        if (!is_sorted(arr, arr_size)) {
            printf("Not Sorted Array while using %s!\n", impl);
            return -1;
        }

        // accumulate time for avg
        total_time += (end_time - start_time);

        delete[] arr;
        delete[] temp_buffer;
    }

    double avg = total_time / repeats;
    return avg;
}





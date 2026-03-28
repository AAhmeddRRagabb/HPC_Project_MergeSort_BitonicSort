
#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>

// geneating arrays
void generate_uniform(int* arr, int n);
void generate_gaussian(int* arr, int n);
void generate_nearly_sorted(int* arr, int n);
void generate_reversed(int* arr, int n);

// utils
bool is_sorted(int* arr, int n);
double get_time();

// MergeSort
void merge(int arr[], int l, int m, int r, int temp[]);
void mergeSort(int arr[], int l, int r, int temp[]);
void mergeSortParallel(int arr[], int l, int r, int temp[]);
double measure_merge_sort(int repeats, int threads, int arr_size, char* impl, char* array_distribution, int seed);


// BitonicSort
#ifdef __CUDACC__
__global__ void bitonicSortKernel(int* arr, int padded_n, int j, int k);
#endif
double runBitonicSort(int* h_arr, int n, int blockSize);
double measure_bitonic_sort(int repeats, int arr_size, char* array_distribution, int seed, int blockSize);


// logging
void log_toolchain_info(FILE *f);
void log_omp_info(FILE *f);
void log_cuda_info(FILE *f);


#endif
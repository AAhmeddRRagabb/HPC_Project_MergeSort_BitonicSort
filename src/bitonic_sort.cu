#include <cuda_runtime.h>
#include <iostream>
#include <climits>
#include "headers.h"
using namespace std;


__global__ void bitonicSortKernel(int* arr, int padded_n, int j, int k) {
    unsigned int i = threadIdx.x + blockDim.x * blockIdx.x;
    
    if (i >= padded_n) return;

    unsigned int ij = i ^ j; 

    if (ij > i) {
        bool ascending = ((i & k) == 0);
        int val_i = arr[i];
        int val_ij = arr[ij];

        // swap
        if (ascending) {
            if (val_i > val_ij) {
                arr[i] = val_ij;
                arr[ij] = val_i;
            }
        } else {
            if (val_i < val_ij) {
                arr[i] = val_ij;
                arr[ij] = val_i;
            }
        }
    }
}


double runBitonicSort(int* h_arr, int n, int block_size) {
    // left shifting to pad [find the next power_2 number to]
    int padded_n = 1;
    while (padded_n < n) padded_n <<= 1; 

    int* d_arr;
    size_t padded_size = padded_n * sizeof(int);

    // cuda mem
    cudaMalloc(&d_arr, padded_size);
    cudaMemset(d_arr, 0x7F, padded_size); 
    cudaMemcpy(d_arr, h_arr, n * sizeof(int), cudaMemcpyHostToDevice);

    // time
    cudaEvent_t start, stop;
    cudaEventCreate(&start); cudaEventCreate(&stop);
    cudaEventRecord(start);

    // bitonic network
    /*
        k -> sub-seq size
        j -> distance to partner
    */
    for (int k = 2; k <= padded_n; k <<= 1) {
        for (int j = k >> 1; j > 0; j >>= 1) {
            int gridSize = (padded_n + block_size - 1) / block_size;
            bitonicSortKernel<<<gridSize, block_size>>>(d_arr, padded_n, j, k);
        }
    }

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float ms = 0;
    cudaEventElapsedTime(&ms, start, stop);

    // cleanup CUDA
    cudaMemcpy(h_arr, d_arr, n * sizeof(int), cudaMemcpyDeviceToHost);
    cudaFree(d_arr);


    double seconds = (double)ms / 1000.0; 
    return seconds;
}



double measure_bitonic_sort(int repeats, int arr_size, char* array_distribution, int seed, int block_size) {
    double total_time = 0;

    for (int i = 0; i < repeats; i++) {
        srand(seed + i); // to ensure re-producability while mainiting generating different arrays

        // generate array
        int *arr = new int[arr_size];

        if (strcmp(array_distribution, "uniform") == 0) generate_uniform(arr, arr_size);
        else if (strcmp(array_distribution, "gaussian") == 0) generate_gaussian(arr, arr_size);
        else if (strcmp(array_distribution, "nearly_sorted") == 0) generate_nearly_sorted(arr, arr_size);
        else if (strcmp(array_distribution, "reversed") == 0) generate_reversed(arr, arr_size);
        else {
            cout << "Invalid Array Distribution" << endl;
            return -1.0;
        }


        // sort
        total_time += runBitonicSort(arr, arr_size, block_size);

        // ensure it is sorted
        if (!is_sorted(arr, arr_size)) {
            printf("Not Sorted Array while using Cuda\n");
            return -1;
        }

        // free
        delete[] arr;
    }

    double avg = total_time / repeats;
    return avg;
}
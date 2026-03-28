#include <cstdlib>  
#include <cmath>     
#include <algorithm>
#include "headers.h"

void generate_uniform(int* arr, int n) {
    for (int i = 0; i < n; i++) arr[i] = rand() % 100000;
}


// Gaussian (Box-Muller)
void generate_gaussian(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        double u1 = (rand() + 1.0) / (RAND_MAX + 1.0);
        double u2 = (rand() + 1.0) / (RAND_MAX + 1.0);
        double z = sqrt(-2.0 * log(u1)) * cos(2 * M_PI * u2);
        arr[i] = (int)(z * 1000 + 50000); // mean ~50000, std ~1000
    }
}

// Nearly sorted
void generate_nearly_sorted(int* arr, int n) {
    // generate sorted
    for (int i = 0; i < n; i++) arr[i] = i;

    // small fraction swapped
    for (int i = 0; i < n / 100; i++) { 
        int a = rand() % n, b = rand() % n;
        std::swap(arr[a], arr[b]);
    }
}

// Reversed
void generate_reversed(int* arr, int n) {
    for (int i = 0; i < n; i++) arr[i] = n - i;
}

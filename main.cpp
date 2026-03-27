#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <cstdlib>
#include <fstream>
#include "mainHeader.h"
using namespace std;

#define ARR_SIZE 10000000

int main(int argc, char* argv[]) {
    /* Parameters */

    // default
    int size = 1000000;
    int threads = 1;
    int repeats = 5;
    char impl[10] = "serial";
    const char* output_path = "results.csv";

    // get terminal args
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--size") == 0) size = atoi(argv[++i]);
        else if (strcmp(argv[i], "--threads") == 0) threads = atoi(argv[++i]);
        else if (strcmp(argv[i], "--impl") == 0) strcpy(impl, argv[++i]);
        else if (strcmp(argv[i], "--repeats") == 0) repeats  = atoi(argv[++i]);
        else if (strcmp(argv[i], "--output") == 0) output_path = argv[++i];
    }

    omp_set_num_threads(threads);


    // ---------------------------------------------------------------------
    /* Measuring */
    double avg = measure_merge_sort(repeats, size, impl);
    
    
    printf("Impl: %s | Size: %d | Threads: %d | Avg Time: %f\n",
           impl, size, threads, avg);



    // ---------------------------------------------------------------------
    /* Saving to results */
    FILE *f = fopen(output_path, "a");
    fprintf(f, "%s,%d,%d,%f\n", impl, size, threads, avg);
    fclose(f);
    

    return 0;
}




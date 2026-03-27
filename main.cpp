#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <cstdlib>
#include <fstream>
#include "src/headers.h"
using namespace std;

#define ARR_SIZE 10000000

int main(int argc, char* argv[]) {
    /* Parameters */

    // default
    int seed = 42;
    int size = 1000000;
    int threads = 1;
    int repeats = 5;
    const char* output_path = "info/results.csv";
    char sort_method[10] = "merge";
    std::string impl = "serial";
    std::string distribution = "uniform";

    // get terminal args
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--size") == 0) size = atoi(argv[++i]);
        else if (strcmp(argv[i], "--seed") == 0) seed = atoi(argv[++i]);
        else if (strcmp(argv[i], "--threads") == 0) threads = atoi(argv[++i]);
        else if (strcmp(argv[i], "--impl") == 0) impl = argv[++i];
        else if (strcmp(argv[i], "--repeats") == 0) repeats  = atoi(argv[++i]);
        else if (strcmp(argv[i], "--output") == 0) output_path = argv[++i];
        else if (strcmp(argv[i], "--distribution") == 0) distribution = argv[++i];
    }

    


    // ---------------------------------------------------------------------
    /* Measuring */
    double avg = 0;
    avg = measure_merge_sort(repeats, threads, size, (char*)impl.c_str(), (char*)distribution.c_str(), seed);
    
    
    
    printf("Impl: %s | Size: %d | ArrayDistribution: %s | Threads: %d | Avg Time: %f\n",
           impl.c_str(), size, distribution.c_str(), threads, avg);



    // ---------------------------------------------------------------------
    /* Saving to results */
    FILE *f = fopen(output_path, "a");
    fprintf(f, "%s,%d,%s,%d,%f\n", impl.c_str(), size, distribution.c_str(), threads, avg);
    fclose(f);
    

    return 0;
}






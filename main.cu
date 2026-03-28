#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cuda_runtime.h>

#include "src/headers.h"
using namespace std;




int main(int argc, char* argv[]) {
    /* Parameters */

    // default
    int seed = 42;
    int repeats = 5;
    int size = 1000000;
    std::string impl = "serial";
    std::string distribution = "uniform";

    int threads = 0;
    int block_size = 0;
    
    const char* output_path = "results/running_results.csv";
    const char* logs_path = "results/system_logs.txt";
    int run_id = 1;
    
    // get terminal args
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--size") == 0) size = atoi(argv[++i]);  // ascii to integer
        else if (strcmp(argv[i], "--seed") == 0) seed = atoi(argv[++i]);
        else if (strcmp(argv[i], "--impl") == 0) impl = argv[++i];
        else if (strcmp(argv[i], "--distribution") == 0) distribution = argv[++i];
        else if (strcmp(argv[i], "--repeats") == 0) repeats  = atoi(argv[++i]);
        
        else if (strcmp(argv[i], "--threads") == 0) threads = atoi(argv[++i]);
        else if (strcmp(argv[i], "--blocksize") == 0) block_size = atoi(argv[++i]);

        else if (strcmp(argv[i], "--output") == 0) output_path = argv[++i];
        else if (strcmp(argv[i], "--logs") == 0) logs_path = argv[++i];
        else if (strcmp(argv[i], "--run_id") == 0) run_id = atoi(argv[++i]);
    }

    // ---------------------------------------------------------------------
    /* Measuring */
    double avg = 0;

    if (impl == "cuda") {
        avg = measure_bitonic_sort(repeats, size, (char*)distribution.c_str(), seed, block_size);
    } else {
        avg = measure_merge_sort(repeats, threads, size, (char*)impl.c_str(), (char*)distribution.c_str(), seed);
    }

    
    if (avg == -1) {
        return 0;
    }

    // ---------------------------------------------------------------------
    /* Saving results */

    // System Info & Current Run Results
    FILE *logging_file = fopen(logs_path, "a");
    fprintf(logging_file, ">> RunID: %d\n\n", run_id);

    fprintf(logging_file, ">> Running Details:\n");
    fprintf(logging_file, "- Implementation: %s\n", impl.c_str());
    fprintf(logging_file, "- Repeats: %d\n", repeats);
    fprintf(logging_file, "- Arr Size: %d\n", size);
    fprintf(logging_file, "- Arr Distribution: %s\n", distribution.c_str());
    fprintf(logging_file, "- Threads: %d\n", threads);
    fprintf(logging_file, "- Blocksize: %d\n", block_size);
    fprintf(logging_file, "- Avg Time: %f\n\n", avg);


    if (impl == "serial") {
        log_toolchain_info(logging_file);
    } else if (impl == "omp") {
        log_toolchain_info(logging_file);
        log_omp_info(logging_file);
    } else if (impl == "cuda") {
        log_toolchain_info(logging_file);
        log_cuda_info(logging_file);
    } 

    fprintf(logging_file, "---------------------------\n");


    // Running Results (.csv)
    FILE *results_file = fopen(output_path, "a");
    fprintf(results_file, "%s,%d,%d,%s,%d,%d,%f\n", 
        impl.c_str(), repeats, size, distribution.c_str(), threads, block_size, avg);


    fclose(logging_file);
    fclose(results_file);

    return 0;
}


void log_toolchain_info(FILE *f) {
    fprintf(f, ">> Toolchain:\n");

    #ifdef __NVCC__
        fprintf(f, "- Device Compiler: NVCC (CUDA Compiler)\n");
    #endif

    #ifdef __GNUC__
        fprintf(f, "- Host Compiler: GCC %d.%d\n", __GNUC__, __GNUC_MINOR__);
    #endif

    #ifdef __linux__
        fprintf(f, "- OS: Linux System");
    #endif

    fprintf(f, "\n\n");
}


void log_omp_info(FILE *f) {
    fprintf(f, ">> OpenMP (CPU) Info:\n");
    fprintf(f, "- Logical Cores Available: %d\n", omp_get_num_procs());
    fprintf(f, "- Max OpenMP Threads: %d\n\n", omp_get_max_threads());
}


void log_cuda_info(FILE *f) {
    int deviceCount;

    cudaGetDeviceCount(&deviceCount);
    if (deviceCount == 0) return;

    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, 0);

    fprintf(f, ">> CUDA (GPU) Log: \n");
    fprintf(f, "- GPU Model: %s\n", prop.name);
    fprintf(f, "- VRAM: %zu MB\n", prop.totalGlobalMem / (1024 * 1024));
    fprintf(f, "- Shared Memory per Block: %zu KB\n", prop.sharedMemPerBlock / 1024);
    fprintf(f, "- Compute Capability: %d.%d\n\n", prop.major, prop.minor);
}
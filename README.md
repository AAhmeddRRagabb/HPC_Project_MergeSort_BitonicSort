# HPC Sorting Analysis: Serial vs. OpenMP vs. CUDA

A comprehensive High-Performance Computing (HPC) project that benchmarks and analyzes the performance of various sorting algorithms across different architectural paradigms. This project compares a baseline **Serial Merge Sort**, a task-parallel **OpenMP Merge Sort**, and a massively parallel **CUDA Bitonic Sort**.

## 🚀 Overview
The goal of this study is to measure execution time, speedup, and scaling efficiency across:
- **4 Array Sizes:** 1M, 4M, 10M, and 16.7M elements.
- **4 Data Distributions:** Uniform, Gaussian, Nearly Sorted, and Reversed.
- **Varying Parallelism:** 1-16 OpenMP threads and 128-512 CUDA block sizes.

## 🛠️ Toolchain & Environment
- **OS:** Linux (Ubuntu 22.04/24.04 recommended)
- **Compiler:** `nvcc` (CUDA 12.x), `gcc` (with OpenMP support)
- **GPU:** NVIDIA RTX 3060 Laptop GPU (Tested)
- **Language:** C++ / CUDA C

## 📁 Project Structure
```text
├── main.cu                # Main entry point and CLI argument parser
├── src/
│   ├── headers.h          # Function prototypes and definitions
│   ├── merge_sort.cpp     # Serial and OpenMP Merge Sort implementations
│   ├── bitonic_sort.cu    # CUDA Kernel and Host wrappers
│   ├── arr_generation.cpp # Logic for generating data distributions
│   └── utils.cpp          # Timing and verification utilities
├── results/               # Output directory for CSV logs and system info
└── run_experiments.sh     # Automation script for 144-run test suite
```

## 📁 CLI Options
Option,Type,Description,Default
--impl,string,"Implementation: serial, omp, or cuda",serial
--size,int,Number of elements in the array,1000000
--distribution,string,"Data pattern: uniform, gaussian, nearly_sorted, reversed",uniform
--repeats,int,Number of times to repeat the experiment for averaging,5
--threads,int,(OpenMP only) Number of CPU threads to use,0
--blocksize,int,(CUDA only) Number of threads per GPU block,0
--output,path,Path to the .csv file to append results,results/running_results.csv
--logs,path,Path to the .txt file for hardware/system logs,results/system_logs.txt
--run_id,int,Unique experiment ID for tracking in logs,1

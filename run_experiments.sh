#!/bin/bash

nvcc -Xcompiler -fopenmp main.cu src/arr_generation.cpp src/merge_sort.cpp src/utils.cpp src/bitonic_sort.cu -o app.exe


ARR_SIZES=(1048576 4194304 16777216 10000000)
ARR_DISTRIBUTIONS=("uniform" "gaussian" "nearly_sorted" "reversed")
OMP_THREADS=(1 2 4 8 16)
CUDA_BLOCKSIZES=(128 256 512)
RUN_ID=1 


echo ">> Starting HPC Project ..."
echo "implementation,n_repeats,array_size,array_distribution,n_threads,blocksize,avg_time" > results/running_results.csv


# Serial
# n_experiments = 4 * 4 = 16

echo ">> Starting Serial Experiments ..."
echo "--- Starting Serial Experiments ---" >> results/system_logs.txt
echo -e "\n" >> results/system_logs.txt

for s in "${ARR_SIZES[@]}"; do
    for d in "${ARR_DISTRIBUTIONS[@]}"; do
        ./app.exe --impl serial --size $s --distribution $d --repeats 5 --output results/running_results.csv --logs results/system_logs.txt --run_id $RUN_ID
        ((RUN_ID++))
    done
done

# -----------------------------------------------------------------------------------------------------------------------------------

# omp
# n_experiments = 4 * 4 * 5_threads = 80

echo ">> Starting OpenMP Experiments ..."

echo "-----------------------------------------------------------------------------------------------" >> results/system_logs.txt
echo "-----------------------------------------------------------------------------------------------" >> results/system_logs.txt
echo "--- Starting OpenMP Experiments ---" >> results/system_logs.txt

echo -e "\n" >> results/system_logs.txt

for s in "${ARR_SIZES[@]}"; do
    for d in "${ARR_DISTRIBUTIONS[@]}"; do
        for t in "${OMP_THREADS[@]}"; do
            ./app.exe --impl omp --size $s --distribution $d --repeats 5 --threads $t --output results/running_results.csv --logs results/system_logs.txt --run_id $RUN_ID
            ((RUN_ID++))
        done
    done
done


# -----------------------------------------------------------------------------------------------------------------------------------

# Cuda
# n_experiments = 4 * 4 * 3_blocksizes = 48

echo ">> Starting CUDA Experiments ..."

echo "-----------------------------------------------------------------------------------------------" >> results/system_logs.txt
echo "-----------------------------------------------------------------------------------------------" >> results/system_logs.txt

echo "--- Starting CUDA Experiments ---" >> results/system_logs.txt
echo -e "\n" >> results/system_logs.txt

for s in "${ARR_SIZES[@]}"; do
    for d in "${ARR_DISTRIBUTIONS[@]}"; do
        for b in "${CUDA_BLOCKSIZES[@]}"; do
            ./app.exe --impl cuda --size $s --distribution $d --repeats 5 --blocksize $b --output results/running_results.csv --logs results/system_logs.txt --run_id $RUN_ID
            ((RUN_ID++))
        done
    done
done
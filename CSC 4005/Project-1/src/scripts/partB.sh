#!/bin/bash

# Get the current directory
CURRENT_DIR=$(pwd)/src/scripts
WORK_DIR=$(pwd)
echo "Current directory: ${CURRENT_DIR}"
echo "Working directory: ${WORK_DIR}"
echo ""

rm -rf ${WORK_DIR}/result/PartB
mkdir -p ${WORK_DIR}/result/PartB/pthread
mkdir -p ${WORK_DIR}/result/PartB/OpenACC



# Sequential PartB
echo "Sequential PartB (Optimized with -O3)"
${WORK_DIR}/build/src/cpu/sequential_PartB ${WORK_DIR}/benchmark.jpg ${WORK_DIR}/result/PartB/sequential.jpg
echo ""

# SIMD PartB
echo "SIMD(AVX2) PartB (Optimized with -O3)"
${WORK_DIR}/build/src/cpu/simd_PartB ${WORK_DIR}/benchmark.jpg ${WORK_DIR}/result/PartB/SIMD.jpg
echo ""

# MPI PartB
echo "MPI PartB (Optimized with -O3)"
for num_cores in 1 2 4 8 16 32
do
  echo "Number of cores: $num_cores"
  mpirun -np ${num_cores} ${WORK_DIR}/build/src/cpu/mpi_PartB ${WORK_DIR}/benchmark.jpg ${WORK_DIR}/result/PartB/MPI.jpg
  echo ""
done

# Pthread PartB
echo "Pthread PartB (Optimized with -O3)"
for num_cores in 1 2 4 8 16 32
do
  echo "Number of cores: $num_cores"
  ${WORK_DIR}/build/src/cpu/pthread_PartB ${WORK_DIR}/benchmark.jpg ${WORK_DIR}/result/PartB/pthread/${num_cores}.jpg ${num_cores}
  echo ""
done

# OpenMP PartB
echo "OpenMP PartB (Optimized with -O3)"
${WORK_DIR}/build/src/cpu/openmp_PartB ${WORK_DIR}/benchmark.jpg ${WORK_DIR}/result/PartB/OpenMP.jpg 16
echo ""

# CUDA PartB
echo "CUDA PartB"
${WORK_DIR}/build/src/gpu/cuda_PartB ${WORK_DIR}/benchmark.jpg ${WORK_DIR}/result/PartB/CUDA.jpg
echo ""

# OpenACC PartB
echo "OpenACC PartB"
for thread in 32 64 128 256 512 1024
do
  echo "Number of workers: $thread"
  ${WORK_DIR}/build/src/gpu/openacc_PartB ${WORK_DIR}/benchmark.jpg ${WORK_DIR}/result/PartB/OpenACC/${thread}.jpg ${thread}
  echo ""
done

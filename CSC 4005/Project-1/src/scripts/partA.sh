#!/bin/bash

# Get the current directory
CURRENT_DIR=$(pwd)/src/scripts
WORK_DIR=$(pwd)
echo "Current directory: ${CURRENT_DIR}"
echo "Working directory: ${WORK_DIR}"

rm -rf ${WORK_DIR}/result/PartA
mkdir -p ${WORK_DIR}/result/PartA/pthread


# Sequential PartA
echo "Sequential PartA (Optimized with -O2)"
${WORK_DIR}/build/src/cpu/sequential_PartA ${WORK_DIR}/benchmark.jpg ${WORK_DIR}/result/PartA/sequential.jpg
echo ""

# SIMD PartA
echo "SIMD(AVX2) PartA (Optimized with -O2)"
${WORK_DIR}/build/src/cpu/simd_PartA ${WORK_DIR}/benchmark.jpg ${WORK_DIR}/result/PartA/SIMD.jpg
echo ""

# MPI PartA
echo "MPI PartA (Optimized with -O2)"
${WORK_DIR}/build/src/cpu/mpi_PartA ${WORK_DIR}/benchmark.jpg ${WORK_DIR}/result/PartA/MPI.jpg
echo ""

# Pthread PartA
echo "Pthread PartA (Optimized with -O2)"
for num_cores in 1 2 4 8 16 32
do
  echo "Number of cores: $num_cores"
  ${WORK_DIR}/build/src/cpu/pthread_PartA ${WORK_DIR}/benchmark.jpg ${WORK_DIR}/result/PartA/pthread/${num_cores}.jpg ${num_cores}
  echo ""
done

# OpenMP PartA
echo "OpenMP PartA (Optimized with -O2)"
${WORK_DIR}/build/src/cpu/openmp_PartA ${WORK_DIR}/benchmark.jpg ${WORK_DIR}/result/PartA/OpenMP.jpg
echo ""

# CUDA PartA
echo "CUDA PartA"
${WORK_DIR}/build/src/gpu/cuda_PartA ${WORK_DIR}/benchmark.jpg ${WORK_DIR}/result/PartA/CUDA.jpg
echo ""

# OpenACC PartA
echo "OpenACC PartA"
${WORK_DIR}/build/src/gpu/openacc_PartA ${WORK_DIR}/benchmark.jpg ${WORK_DIR}/result/PartA/OpenACC.jpg
echo ""

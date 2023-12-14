#!/bin/bash
#SBATCH -o /nfsmnt/120090645/project3/build/Project3.txt
#SBATCH -p Project
#SBATCH -J Project3
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
#SBATCH --gres=gpu:1

scancel -u 120090645

# quicksort
echo "Quick Sort MPI (Optimized with -O2)"
for num_cores in 1 2 4 8 16 32
do
  echo "Number of cores: $num_cores"
  srun -n $num_cores --cpus-per-task 1 --mpi=pmi2 perf stat -e cpu-cycles,cache-misses,page-faults /nfsmnt/120090645/project3/build/src/quicksort/quicksort_mpi 100000000
  echo ""
  echo ""
  echo ""
  echo ""
done
echo ""
echo ""


# bucket sort
echo "Bucket Sort MPI (Optimized with -O2)"
for num_cores in 1 2 4 8 16 32
do
  echo "Number of cores: $num_cores"
  srun -n $num_cores --cpus-per-task 1 --mpi=pmi2 perf stat -e cpu-cycles,cache-misses,page-faults /nfsmnt/120090645/project3/build/src/bucketsort/bucketsort_mpi 100000000 1000000
  echo ""
  echo ""
  echo ""
  echo ""
done
echo ""
echo ""


# odd-even sort
echo "Odd-Even Sort MPI (Optimized with -O2)"
for num_cores in 1 2 4 8 16 32
do
  echo "Number of cores: $num_cores"
  srun -n $num_cores --cpus-per-task 1 --mpi=pmi2 perf stat -e cpu-cycles,cache-misses,page-faults /nfsmnt/120090645/project3/build/src/odd-even-sort/odd-even-sort_mpi 200000
  echo ""
  echo ""
  echo ""
  echo ""
done
echo ""
echo ""

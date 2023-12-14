# Report

* Author: Haopeng Chen
* ID: 120090645

## How to Start

### Compile

```bash
cmake -B build
cmake --build build
```

### Execution

```bash
# on cluster
sh src/scripts/sbatch_PartB.sh
```

## Result

### Execution Time

![image-20231009182748579](./assets/image-20231009182748579.png)

*Note: CUDA and OpenACC are using fixed number of workers. SIMD is using fixed number of CPU cores.*

### Speed Up Factor

| Type \  Core(s) |     1 |     2 |     4 |     8 |    16 |    32 |
| --------------- | ----: | ----: | ----: | ----: | ----: | ----: |
| **Sequential**  |  1.00 |  1.00 |  1.00 |  1.00 |  1.00 |  1.00 |
| **SIMD (AVX2)** |  4.42 |  4.42 |  4.42 |  4.42 |  4.42 |  4.42 |
| **MPI**         |  0.98 |  1.40 |  2.62 |  4.66 |  6.76 | 11.28 |
| **Pthread**     |  1.11 |  1.23 |  2.19 |  4.34 |  9.42 | 17.44 |
| **OpenMP**      |  1.26 |  1.26 |  2.26 |  4.53 | 10.05 | 18.80 |
| **CUDA**        | 39.55 | 39.55 | 39.55 | 39.55 | 39.55 | 39.55 |
| **OpenACC**     |  8.75 |  8.75 |  8.75 |  8.75 |  8.75 |  8.75 |

*Note: CUDA and OpenACC are using fixed number of workers. SIMD is using fixed number of CPU cores.*

### Efficiency

| Type \  Core(s) |     1 |     2 |     4 |     8 |    16 |    32 |
| :-------------- | ----: | ----: | ----: | ----: | ----: | ----: |
| **Sequential**  |  100% |  100% |  100% |  100% |  100% |  100% |
| **SIMD (AVX2)** |  442% |  442% |  442% |  442% |  442% |  442% |
| **MPI**         |   98% |   70% |   66% |   58% |   42% |   35% |
| **Pthread**     |  111% |   61% |   55% |   54% |   59% |   54% |
| **OpenMP**      |  126% |   63% |   57% |   57% |   63% |   59% |
| **CUDA**        | 3955% | 3955% | 3955% | 3955% | 3955% | 3955% |
| **OpenACC**     |  875% |  875% |  875% |  875% |  875% |  875% |

*Note: CUDA and OpenACC are using fixed number of workers and treated as using 1 core. SIMD is using a fixed number of CPU cores, i.e. 1.*

## Analysis

To begin with, I have to emphasize that due to the fluctuance of cluster performance, though I have tried my best, the experiment results may **not be accurate enough**.

* **Parallelism Introduction**
  * SIMD (AVX2): **DLP**. We extend from scaler to vector, that is, we could manipulate 8 scaler values at the same time. A convolution could be separated two 2 types of operations, one is multiplication, the other is addition. We need to make one-to-one multiplication of source data and filter of 9 times, and sum these results together. In SIMD, we could still keep each convolutions serial, but make parallelism among each convolution. That is, inside one convolution, it could still be considered using a scaler. But in general, we manipulate 8 convolutions simultaneously.
  * MPI: MPI should be considered as a mechanism of multiple processes, not related to ILP, DLP or TLP. As mentioned above, each convolution is independent. And we could aggregate several convolutions into a task, which is finished by an independent process. We could apply MPI to incorporate multiple processes at the same time.
  * Pthread: **TLP**. Similar to MPI, we combine several convolutions into one task. And we allocate each task a thread, while sharing the memory space, global variables and etc.![img](./assets/thread.jpg)
  * OpenMP: **TLP**. Very similar to Pthread, just another API.
  * CUDA: **DLP**. It enables developers to write code that can be executed concurrently on a massive number of parallel processing units within a GPU, aka. parallel execution of the same instruction (or a kernel) on a large amount of data by utilizing thousands of threads running on GPU cores in parallel. We define each convolution as one task, which contributes to millions of simple tasks, utilizing the advantages of CUDA.
  * OpenACC: **DLP**. Very similar with CUDA but supporting various devices, instead of NVIDIA GPUs only. It is a set of directives and compiler directives designed to simplify parallel programming for heterogeneous computing systems, including GPUs and other accelerators. We keep the implementation aligned with CUDA one mostly, only with grammar differences.

* **Optimization Approaches**
  * The program involves many array read and write access. Try to distribute data closely to enrich cache-hit, reducing waiting time for memory.
  * Change from `O2` to `O3`, which introduce following flags:
    * **`-fgcse-after-reload`**:
      - Function: Performs Global Common Subexpression Elimination (GCSE) after the reload phase.
      - Explanation: Attempts to eliminate redundant computations by identifying and reusing common subexpressions across the program.
    * **`-fipa-cp-clone`**:
      - Function: Performs interprocedural constant propagation using cloning.
      - Explanation: Propagates constant values across different functions, which can lead to further optimizations and improved performance.
    * **`-floop-interchange`**:
      - Function: Enables loop interchange optimization.
      - Explanation: Reorders nested loops to improve data locality and cache performance.
    * **`-floop-unroll-and-jam`**:
      - Function: Unrolls loops and then fuses adjacent loop iterations.
      - Explanation: Combines loop unrolling with loop fusion to improve performance by reducing loop overhead and increasing instruction-level parallelism.
    * **`-fpeel-loops`**:
      - Function: Unrolls small loops.
      - Explanation: Unrolls loops with a small number of iterations, aiming to improve performance by reducing loop overhead.
    * **`-fpredictive-commoning`**:
      - Function: Performs predictive commoning optimization.
      - Explanation: Identifies and commonizes expressions across loops to reduce redundant computations and improve performance.
    * **`-fsplit-loops`**:
      - Function: Splits loops into multiple loops.
      - Explanation: Divides large loops into smaller loops, potentially improving cache performance and reducing loop overhead.
    * **`-fsplit-paths`**:
      - Function: Splits critical paths in the control flow graph.
      - Explanation: Divides critical paths in the control flow graph into simpler segments, allowing for improved instruction scheduling and optimization.
    * **`-ftree-loop-distribution`**:
      - Function: Distributes loops across functions.
      - Explanation: Attempts to distribute loop code across functions to improve parallelism and optimization opportunities.
    * **`-ftree-partial-pre`**:
      - Function: Enables partial redundancy elimination (PRE) optimization.
      - Explanation: Eliminates redundant computations and expressions to optimize the program.
    * **`-funswitch-loops`**:
      - Function: Optimizes by using loop unswitching.
      - Explanation: Unswitches loops by moving conditions outside the loop, improving loop performance by reducing branch prediction penalties.
    * **`-fvect-cost-model=dynamic`**:
      - Function: Sets the vectorization cost model to dynamic.
      - Explanation: Allows the compiler to dynamically adjust the cost model for vectorization based on the target architecture and program characteristics.
    * **`-fversion-loops-for-strides`**:
      - Function: Optimizes loops based on array strides.
      - Explanation: Adjusts loop structure based on array access patterns to improve cache performance and memory access efficiency.
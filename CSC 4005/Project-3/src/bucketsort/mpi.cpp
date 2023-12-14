//
// Created by Yang Yufan on 2023/10/31.
// Email: yufanyang1@link.cuhk.edu.cn
//
// Parallel Bucket Sort with MPI
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <mpi.h>
#include "../utils.hpp"

#define MASTER 0

struct Node
{
    int* val;
    int index;
    int id;
    Node(int* v, int i, int c) : val(v), index(i), id(c) {}
};
struct cmp
{
    bool operator()(const Node& a, const Node& b) { return *a.val > *b.val; }
};

void insertionSort(std::vector<int>& bucket)
{
    for (int i = 1; i < bucket.size(); ++i)
    {
        int key = bucket[i];
        int j = i - 1;

        while (j >= 0 && bucket[j] > key)
        {
            bucket[j + 1] = bucket[j];
            j--;
        }

        bucket[j + 1] = key;
    }
}

void bucketSort(std::vector<int>& vec, int num_buckets)
{
    int max_val = *std::max_element(vec.begin(), vec.end());
    int min_val = *std::min_element(vec.begin(), vec.end());

    int range = max_val - min_val + 1;
    int small_bucket_size = range / num_buckets;
    int large_bucket_size = small_bucket_size + 1;
    int large_bucket_num = range - small_bucket_size * num_buckets;
    int boundary = min_val + large_bucket_num * large_bucket_size;

    std::vector<std::vector<int>> buckets(num_buckets);
    // Pre-allocate space to avoid re-allocation
    for (std::vector<int>& bucket : buckets)
    {
        bucket.reserve(large_bucket_num);
    }

    // Place each element in the appropriate bucket
    for (int num : vec)
    {
        int index;
        if (num < boundary)
        {
            index = (num - min_val) / large_bucket_size;
        }
        else
        {
            index = large_bucket_num + (num - boundary) / small_bucket_size;
        }
        if (index >= num_buckets)
        {
            // Handle elements at the upper bound
            index = num_buckets - 1;
        }
        buckets[index].push_back(num);
    }

    // Sort each bucket using insertion sort
    for (std::vector<int>& bucket : buckets)
    {
        insertionSort(bucket);
    }

    // Combine sorted buckets to get the final sorted array
    int index = 0;
    for (const std::vector<int>& bucket : buckets)
    {
        for (int num : bucket)
        {
            vec[index++] = num;
        }
    }
}

int main(int argc, char** argv)
{
    // Verify input argument format
    if (argc != 3)
    {
        throw std::invalid_argument("Invalid argument, should be: ./executable "
                                    "vector_size bucket_num\n");
    }
    // Start the MPI
    MPI_Init(&argc, &argv);
    // How many processes are running
    int numtasks;
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    // What's my rank?
    int taskid;
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    // Which node am I running on?
    int len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    MPI_Get_processor_name(hostname, &len);
    MPI_Status status;

    int size = atoi(argv[1]);

    const int bucket_num = atoi(argv[2]);

    const int seed = 4005;

    std::vector<int> vec;
    std::vector<int> vec_clone;

    std::vector<int> cuts = createCuts(0, size - 1, numtasks);
    std::vector<int> bucket_cuts = createCuts(0, bucket_num - 1, numtasks);

    int elements_per_proc = cuts[taskid + 1] - cuts[taskid];
    int buckets_per_proc = bucket_cuts[taskid + 1] - bucket_cuts[taskid];

    if (taskid == MASTER)
    {
        vec = createRandomVec(size, seed);
        vec_clone = vec;
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    if (taskid == MASTER)
    {
        std::vector<int> local_vec(elements_per_proc);
        local_vec.assign(vec.begin(), vec.begin() + elements_per_proc);
        bucketSort(local_vec, buckets_per_proc);

        vec.assign(local_vec.begin(), local_vec.end());

        for (int i = 1; i < numtasks; ++i)
        {
            MPI_Send(vec.data() + cuts[i], cuts[i + 1] - cuts[i], MPI_INT, i, 0,
                     MPI_COMM_WORLD);
        }

        // receive the sorted cuts from other processes
        for (int i = 1; i < numtasks; i++)
        {
            MPI_Recv(vec.data() + cuts[i], cuts[i + 1] - cuts[i], MPI_INT, i, 0,
                     MPI_COMM_WORLD, &status);
        }

        // merge the sorted cuts
        std::priority_queue<Node, std::vector<Node>, cmp> pq;
        for (int i = 0; i < numtasks; i++)
        {
            pq.push(Node(&vec[cuts[i]], cuts[i], i));
        }

        int k = 0;
        std::vector<int> res(size);
        while (!pq.empty())
        {
            Node node = pq.top();
            pq.pop();

            res[k++] = *node.val;
            if (node.index + 1 < cuts[node.id + 1])
            {
                pq.push(Node(&vec[node.index + 1], node.index + 1, node.id));
            }
        }
        vec.assign(res.begin(), res.end());

        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                                  start_time);

        std::cout << "Bucket Sort Complete!" << std::endl;
        std::cout << "Execution Time: " << elapsed_time.count()
                  << " milliseconds" << std::endl;

        checkSortResult(vec_clone, vec);
    }
    else
    {
        vec.resize(elements_per_proc);
        MPI_Recv(vec.data(), elements_per_proc, MPI_INT, MASTER, 0,
                 MPI_COMM_WORLD, &status);
        bucketSort(vec, buckets_per_proc);
        MPI_Send(vec.data(), elements_per_proc, MPI_INT, MASTER, 0,
                 MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
//
// Created by Yang Yufan on 2023/10/31.
// Email: yufanyang1@link.cuhk.edu.cn
//
// Parallel Quick Sort with MPI
//

#include <iostream>
#include <vector>
#include <mpi.h>
#include "../utils.hpp"
#include <cmath>
#include <list>
#include <queue>

#define MASTER 0

int partition(std::vector<int>& vec, const int low, const int high)
{
    int pivot = vec[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (vec[j] <= pivot)
        {
            i++;
            std::swap(vec[i], vec[j]);
        }
    }

    std::swap(vec[i + 1], vec[high]);
    return i + 1;
}

void helper(std::vector<int>& vec, const int low, const int high)
{
    if (low < high)
    {
        int pivotIndex = partition(vec, low, high);
        helper(vec, low, pivotIndex - 1);
        helper(vec, pivotIndex + 1, high);
    }
}

void quickSort(std::vector<int>& vec, int numtasks, int taskid,
               MPI_Status* status)
{
    /* Your code here!
       Implement parallel quick sort with MPI
    */

    auto full_size = vec.size();
    auto task_size = full_size / numtasks;
    auto remains = full_size % numtasks;

    if (taskid == MASTER)
    {
        for (auto i = 1; i < numtasks; i++)
        {
            if (i == numtasks - 1)
            {
                MPI_Send(&vec[i * task_size], task_size + remains, MPI_INT, i,
                         0, MPI_COMM_WORLD);
            }
            else
            {
                MPI_Send(&vec[i * task_size], task_size, MPI_INT, i, 0,
                         MPI_COMM_WORLD);
            }
        }
    }
    else
    {
        // init container
        std::vector<int> sub_vec(task_size, 0);

        // recv & set correct size
        if (taskid == numtasks - 1)
        {
            sub_vec.resize(task_size + remains, 0);
            MPI_Recv(&sub_vec[0], task_size + remains, MPI_INT, MASTER, 0,
                     MPI_COMM_WORLD, status);
        }
        else
        {
            MPI_Recv(&sub_vec[0], task_size, MPI_INT, MASTER, 0, MPI_COMM_WORLD,
                     status);
        }

        // Do sorting
        helper(sub_vec, 0, sub_vec.size() - 1);

        // Send back
        MPI_Send(&sub_vec[0], sub_vec.size(), MPI_INT, MASTER, 0,
                 MPI_COMM_WORLD);
    }

    if (taskid == MASTER)
    {
        std::vector<std::vector<int>> tmp{};

        // Do master work first
        std::vector<int> res1(vec.begin(), vec.begin() + task_size);
        helper(res1, 0, res1.size() - 1);
        tmp.push_back(res1);

        // Recv slave work
        for (auto i = 1; i < numtasks; i++)
        {
            if (i == numtasks - 1)
            {
                std::vector<int> sub_vec(task_size + remains, 0);
                MPI_Recv(&sub_vec[0], task_size + remains, MPI_INT, i, 0,
                         MPI_COMM_WORLD, status);
                tmp.push_back(sub_vec);
            }
            else
            {
                std::vector<int> sub_vec(task_size, 0);
                MPI_Recv(&sub_vec[0], task_size, MPI_INT, i, 0, MPI_COMM_WORLD,
                         status);
                tmp.push_back(sub_vec);
            }
        }

        // Fast-track: if one process, no need to merge
        if (numtasks == 1)
        {
            vec = tmp[0];
            return;
        }

        std::vector<int> result(vec.size());

        auto time1 = std::chrono::high_resolution_clock::now();

        typedef std::pair<int*, int> pair; // (&value, index)
        // Compare function, smaller one first
        auto cmp = [](const pair& a, const pair& b) {
            return *a.first > *b.first;
        };

        // Merge
        std::priority_queue<pair, std::vector<pair>, decltype(cmp)> pq(cmp);

        for (auto i = 0; i < numtasks; i++)
        {
            pq.emplace(&tmp[i][0], i); // push first element into pq
        }

        auto index = 0;
        while (!pq.empty())
        {
            auto top = pq.top();
            pq.pop();
            result[index++] = *top.first;
            auto next_pos = top.first + 1;
            auto subvec_index = top.second;
            if (next_pos < &tmp[subvec_index][0] + tmp[subvec_index].size())
            {
                pq.emplace(next_pos, subvec_index);
            }
        }

        auto time2 = std::chrono::high_resolution_clock::now();
        auto elapsed_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(time2 -
                                                                  time1);
        std::cout << "Merge Time: " << elapsed_time.count() << " milliseconds"
                  << std::endl;

        vec = result;
    }
}

int main(int argc, char** argv)
{
    // Verify input argument format
    if (argc != 2)
    {
        throw std::invalid_argument(
            "Invalid argument, should be: ./executable vector_size\n");
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

    const int size = atoi(argv[1]);

    const int seed = 4005;

    std::vector<int> vec = createRandomVec(size, seed);
    std::vector<int> vec_clone = vec;

    auto start_time = std::chrono::high_resolution_clock::now();

    quickSort(vec, numtasks, taskid, &status);

    if (taskid == MASTER)
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                                  start_time);

        std::cout << "Quick Sort Complete!" << std::endl;
        std::cout << "Execution Time: " << elapsed_time.count()
                  << " milliseconds" << std::endl;

        checkSortResult(vec_clone, vec);
    }

    MPI_Finalize();
    return 0;
}
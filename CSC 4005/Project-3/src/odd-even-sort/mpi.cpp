//
// Created by Yang Yufan on 2023/10/31.
// Email: yufanyang1@link.cuhk.edu.cn
//
// Parallel Odd-Even Sort with MPI
//

#include <iostream>
#include <vector>
#include <mpi.h>
#include <algorithm>
#include <cstring>
#include "../utils.hpp"

#define MASTER 0

bool isEdge(int taskid, int odd_even, bool last_rank)
{
    if (odd_even == 0)
        return (taskid & 1) == 0 && last_rank;
    else
        return taskid == 0 || ((taskid & 1) == 1 && last_rank);
}

// merge adj_data and data, take the larger element, store in cp_data
void mergeLeft(std::vector<int>& vec, std::vector<int>& adj_vec,
               std::vector<int>& cp_vec)
{
    int* p1 = vec.data();
    int* vec_end = vec.data() + vec.size();
    int* p2 = adj_vec.data();
    int* adj_vec_end = adj_vec.data() + adj_vec.size();
    int* p = cp_vec.data();
    int* cp_vec_end = cp_vec.data() + cp_vec.size();

    while (p != cp_vec_end && p1 != vec_end && p2 != adj_vec_end)
    {
        *(p++) = ((*p1) <= (*p2)) ? *(p1++) : *(p2++);
    }
    while (p != cp_vec_end)
    {
        *(p++) = *(p1++);
    }
}

// merge data and adj_data, take the smaller element, store in cp_data
void mergeRight(std::vector<int>& vec, std::vector<int>& adj_vec,
                std::vector<int>& cp_vec)
{
    int* p1 = vec.data() + vec.size();
    int* p2 = adj_vec.data() + adj_vec.size();
    int* p = cp_vec.data() + cp_vec.size();

    while (p != cp_vec.data() && p1 != vec.data() && p2 != adj_vec.data())
    {
        *(--p) = (*(p1 - 1) >= *(p2 - 1)) ? *(--p1) : *(--p2);
    }
    while (p != cp_vec.data())
    {
        *(--p) = *(--p2);
    }
}

void oddEvenSort(std::vector<int>& vec)
{
    bool sorted = false;

    while (!sorted)
    {
        sorted = true;

        // Perform the odd phase
        for (int i = 1; i < vec.size() - 1; i += 2)
        {
            if (vec[i] > vec[i + 1])
            {
                std::swap(vec[i], vec[i + 1]);
                sorted = false;
            }
        }

        // Perform the even phase
        for (int i = 0; i < vec.size() - 1; i += 2)
        {
            if (vec[i] > vec[i + 1])
            {
                std::swap(vec[i], vec[i + 1]);
                sorted = false;
            }
        }
    }
}

void sort(std::vector<int>& local_vec, int numtasks, int taskid,
          MPI_Status* status, std::vector<int>& cuts)
{
    int block_len = local_vec.size();
    oddEvenSort(local_vec);
    if (numtasks == 1) return;

    bool odd_or_even = 0; // 0: even; 1: odd;
    std::vector<int> cp_data(block_len);
    std::vector<int> adj_data(numtasks);

    int limit = numtasks;
    while (limit--)
    {
        if (isEdge(taskid, odd_or_even, (taskid == numtasks - 1)))
        {
            // do nothing
        }
        else if ((taskid & 1) == odd_or_even)
        {
            int adj_block_len = cuts[taskid + 1] - cuts[taskid];
            adj_data.resize(adj_block_len);

            // check if the last element in this block is larger than the first
            MPI_Request request[2];
            MPI_Isend(local_vec.data() + block_len - 1, 1, MPI_INT, taskid + 1,
                      0, MPI_COMM_WORLD, &request[0]);
            MPI_Irecv(adj_data.data(), 1, MPI_INT, taskid + 1, 1,
                      MPI_COMM_WORLD, &request[1]);
            MPI_Wait(&request[0], status);
            MPI_Wait(&request[1], status);

            // last element larger than the first in next block
            if (local_vec[block_len - 1] > adj_data[0])
            {
                MPI_Sendrecv(&local_vec[0], block_len, MPI_INT, taskid + 1, 0,
                             &adj_data[0], adj_block_len, MPI_INT, taskid + 1,
                             1, MPI_COMM_WORLD, status);

                mergeLeft(local_vec, adj_data, cp_data);

                std::copy(cp_data.begin(), cp_data.end(), local_vec.begin());
            }
        }
        else if ((taskid & 1) != odd_or_even)
        {
            int adj_block_len = cuts[taskid] - cuts[taskid - 1];
            adj_data.resize(adj_block_len);

            MPI_Request request[2];
            MPI_Isend(local_vec.data(), 1, MPI_INT, taskid - 1, 1,
                      MPI_COMM_WORLD, &request[1]);
            MPI_Irecv(adj_data.data() + adj_block_len - 1, 1, MPI_INT,
                      taskid - 1, 0, MPI_COMM_WORLD, &request[0]);
            MPI_Wait(&request[1], status);
            MPI_Wait(&request[0], status);

            if (adj_data[adj_block_len - 1] > local_vec[0])
            {
                MPI_Sendrecv(&local_vec[0], block_len, MPI_INT, taskid - 1, 1,
                             &adj_data[0], adj_block_len, MPI_INT, taskid - 1,
                             0, MPI_COMM_WORLD, status);

                mergeRight(adj_data, local_vec, cp_data);

                std::copy(cp_data.begin(), cp_data.end(), local_vec.begin());
            }
        }
        odd_or_even ^= 1;
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

    std::vector<int> vec;
    std::vector<int> vec_clone;

    std::vector<int> cuts = createCuts(0, size - 1, numtasks);
    std::vector<int> sort_status(numtasks, 0);

    int elements_per_proc;
    elements_per_proc = cuts[taskid + 1] - cuts[taskid];

    if (taskid == MASTER)
    {
        vec = createRandomVec(size, seed);
        vec_clone = vec;
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    if (taskid == MASTER)
    {
        std::vector<int> local_vec(elements_per_proc);
        local_vec.assign(vec.data(), vec.data() + elements_per_proc);
        for (int i = 1; i < numtasks; ++i)
        {
            MPI_Send(vec.data() + cuts[i], cuts[i + 1] - cuts[i], MPI_INT, i, 2,
                     MPI_COMM_WORLD);
        }
        sort(local_vec, numtasks, taskid, &status, cuts);

        std::vector<int> res(size);

        memcpy(vec.data(), local_vec.data(), elements_per_proc * sizeof(int));
        for (int i = 1; i < numtasks; ++i)
        {
            MPI_Recv(vec.data() + cuts[i], cuts[i + 1] - cuts[i], MPI_INT, i, 2,
                     MPI_COMM_WORLD, &status);
        }
    }
    else
    {
        vec.resize(elements_per_proc);
        MPI_Recv(vec.data(), elements_per_proc, MPI_INT, MASTER, 2,
                 MPI_COMM_WORLD, &status);
        sort(vec, numtasks, taskid, &status, cuts);
        MPI_Send(vec.data(), elements_per_proc, MPI_INT, MASTER, 2,
                 MPI_COMM_WORLD);
    }

    if (taskid == MASTER)
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                                  start_time);

        std::cout << "Odd-Even Sort Complete!" << std::endl;
        std::cout << "Execution Time: " << elapsed_time.count()
                  << " milliseconds" << std::endl;

        checkSortResult(vec_clone, vec);
    }

    MPI_Finalize();
    return 0;
}
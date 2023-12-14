//
// Created by Yang Yufan on 2023/10/07.
// Email: yufanyang1@link.cuhk.edu.cn
//
// MPI + OpenMp + SIMD + Reordering Matrix Multiplication
//

#include <mpi.h> // MPI Header
#include <omp.h>
#include <immintrin.h>
#include <stdexcept>
#include <chrono>
#include <cstring>
#include "matrix.hpp"

#define MASTER 0

Matrix matrix_multiply_mpi(const Matrix& matrix1, const Matrix& matrix2,
                           const size_t local_m_start, const size_t local_m_end)
{
    if (matrix1.getCols() != matrix2.getRows())
    {
        throw std::invalid_argument(
            "Matrix dimensions are not compatible for multiplication.");
    }
    size_t K = matrix1.getCols(), N = matrix2.getCols();
    size_t M = local_m_end - local_m_start; // 256
    // [256 x 1024] * [1024 x 1024] = [256 x 1024]
    Matrix result(M, N); // 256 x 1024
    // Your Code Here!
    // Optimizing Matrix Multiplication
    // In addition to OpenMP, SIMD, Memory Locality and Cache Missing,
    // Further Applying MPI
    // Note:
    // You can change the argument of the function
    // for your convenience of task division
    auto m_tile = M / 8 * 8;
    auto n_tile = N / 8 * 8;
    auto k_tile = K / 8 * 8;

    bool m_over = M % 8 != 0;
    bool n_over = N % 8 != 0;
    bool k_over = K % 8 != 0;

#pragma omp parallel for schedule(auto)
    for (size_t i = local_m_start; i < m_tile + local_m_start; i += 8)
    { // start from 256, end at 511
        auto row0 = matrix1[i];
        auto row1 = matrix1[i + 1];
        auto row2 = matrix1[i + 2];
        auto row3 = matrix1[i + 3];
        auto row4 = matrix1[i + 4];
        auto row5 = matrix1[i + 5];
        auto row6 = matrix1[i + 6];
        auto row7 = matrix1[i + 7];

        auto result_row0 = result[i - local_m_start];
        auto result_row1 = result[i + 1 - local_m_start];
        auto result_row2 = result[i + 2 - local_m_start];
        auto result_row3 = result[i + 3 - local_m_start];
        auto result_row4 = result[i + 4 - local_m_start];
        auto result_row5 = result[i + 5 - local_m_start];
        auto result_row6 = result[i + 6 - local_m_start];
        auto result_row7 = result[i + 7 - local_m_start];

        for (size_t k = 0; k < k_tile; k += 8)
        {
            auto col0 = matrix2[k];
            auto col1 = matrix2[k + 1];
            auto col2 = matrix2[k + 2];
            auto col3 = matrix2[k + 3];
            auto col4 = matrix2[k + 4];
            auto col5 = matrix2[k + 5];
            auto col6 = matrix2[k + 6];
            auto col7 = matrix2[k + 7];

            // val00
            __m256i val00v = _mm256_set1_epi32(row0[k]);
            __m256i val01v = _mm256_set1_epi32(row0[k + 1]);
            __m256i val02v = _mm256_set1_epi32(row0[k + 2]);
            __m256i val03v = _mm256_set1_epi32(row0[k + 3]);
            __m256i val04v = _mm256_set1_epi32(row0[k + 4]);
            __m256i val05v = _mm256_set1_epi32(row0[k + 5]);
            __m256i val06v = _mm256_set1_epi32(row0[k + 6]);
            __m256i val07v = _mm256_set1_epi32(row0[k + 7]);

            // val10
            __m256i val10v = _mm256_set1_epi32(row1[k]);
            __m256i val11v = _mm256_set1_epi32(row1[k + 1]);
            __m256i val12v = _mm256_set1_epi32(row1[k + 2]);
            __m256i val13v = _mm256_set1_epi32(row1[k + 3]);
            __m256i val14v = _mm256_set1_epi32(row1[k + 4]);
            __m256i val15v = _mm256_set1_epi32(row1[k + 5]);
            __m256i val16v = _mm256_set1_epi32(row1[k + 6]);
            __m256i val17v = _mm256_set1_epi32(row1[k + 7]);

            // val20
            __m256i val20v = _mm256_set1_epi32(row2[k]);
            __m256i val21v = _mm256_set1_epi32(row2[k + 1]);
            __m256i val22v = _mm256_set1_epi32(row2[k + 2]);
            __m256i val23v = _mm256_set1_epi32(row2[k + 3]);
            __m256i val24v = _mm256_set1_epi32(row2[k + 4]);
            __m256i val25v = _mm256_set1_epi32(row2[k + 5]);
            __m256i val26v = _mm256_set1_epi32(row2[k + 6]);
            __m256i val27v = _mm256_set1_epi32(row2[k + 7]);

            // val30
            __m256i val30v = _mm256_set1_epi32(row3[k]);
            __m256i val31v = _mm256_set1_epi32(row3[k + 1]);
            __m256i val32v = _mm256_set1_epi32(row3[k + 2]);
            __m256i val33v = _mm256_set1_epi32(row3[k + 3]);
            __m256i val34v = _mm256_set1_epi32(row3[k + 4]);
            __m256i val35v = _mm256_set1_epi32(row3[k + 5]);
            __m256i val36v = _mm256_set1_epi32(row3[k + 6]);
            __m256i val37v = _mm256_set1_epi32(row3[k + 7]);

            // val40
            __m256i val40v = _mm256_set1_epi32(row4[k]);
            __m256i val41v = _mm256_set1_epi32(row4[k + 1]);
            __m256i val42v = _mm256_set1_epi32(row4[k + 2]);
            __m256i val43v = _mm256_set1_epi32(row4[k + 3]);
            __m256i val44v = _mm256_set1_epi32(row4[k + 4]);
            __m256i val45v = _mm256_set1_epi32(row4[k + 5]);
            __m256i val46v = _mm256_set1_epi32(row4[k + 6]);
            __m256i val47v = _mm256_set1_epi32(row4[k + 7]);

            // val50
            __m256i val50v = _mm256_set1_epi32(row5[k]);
            __m256i val51v = _mm256_set1_epi32(row5[k + 1]);
            __m256i val52v = _mm256_set1_epi32(row5[k + 2]);
            __m256i val53v = _mm256_set1_epi32(row5[k + 3]);
            __m256i val54v = _mm256_set1_epi32(row5[k + 4]);
            __m256i val55v = _mm256_set1_epi32(row5[k + 5]);
            __m256i val56v = _mm256_set1_epi32(row5[k + 6]);
            __m256i val57v = _mm256_set1_epi32(row5[k + 7]);

            // val60
            __m256i val60v = _mm256_set1_epi32(row6[k]);
            __m256i val61v = _mm256_set1_epi32(row6[k + 1]);
            __m256i val62v = _mm256_set1_epi32(row6[k + 2]);
            __m256i val63v = _mm256_set1_epi32(row6[k + 3]);
            __m256i val64v = _mm256_set1_epi32(row6[k + 4]);
            __m256i val65v = _mm256_set1_epi32(row6[k + 5]);
            __m256i val66v = _mm256_set1_epi32(row6[k + 6]);
            __m256i val67v = _mm256_set1_epi32(row6[k + 7]);

            // val70
            __m256i val70v = _mm256_set1_epi32(row7[k]);
            __m256i val71v = _mm256_set1_epi32(row7[k + 1]);
            __m256i val72v = _mm256_set1_epi32(row7[k + 2]);
            __m256i val73v = _mm256_set1_epi32(row7[k + 3]);
            __m256i val74v = _mm256_set1_epi32(row7[k + 4]);
            __m256i val75v = _mm256_set1_epi32(row7[k + 5]);
            __m256i val76v = _mm256_set1_epi32(row7[k + 6]);
            __m256i val77v = _mm256_set1_epi32(row7[k + 7]);

            __m256i r0 = _mm256_setzero_si256(), r1 = _mm256_setzero_si256(),
                    r2 = _mm256_setzero_si256(), r3 = _mm256_setzero_si256();
            __m256i r4 = _mm256_setzero_si256(), r5 = _mm256_setzero_si256(),
                    r6 = _mm256_setzero_si256(), r7 = _mm256_setzero_si256();

            for (size_t j = 0; j < n_tile; j += 8)
            {
                r0 = _mm256_setzero_si256(), r1 = _mm256_setzero_si256(),
                r2 = _mm256_setzero_si256(), r3 = _mm256_setzero_si256();
                r4 = _mm256_setzero_si256(), r5 = _mm256_setzero_si256(),
                r6 = _mm256_setzero_si256(), r7 = _mm256_setzero_si256();
                auto col0v = _mm256_loadu_si256((__m256i*)&col0[j]);
                auto col1v = _mm256_loadu_si256((__m256i*)&col1[j]);
                auto col2v = _mm256_loadu_si256((__m256i*)&col2[j]);
                auto col3v = _mm256_loadu_si256((__m256i*)&col3[j]);
                auto col4v = _mm256_loadu_si256((__m256i*)&col4[j]);
                auto col5v = _mm256_loadu_si256((__m256i*)&col5[j]);
                auto col6v = _mm256_loadu_si256((__m256i*)&col6[j]);
                auto col7v = _mm256_loadu_si256((__m256i*)&col7[j]);

                r0 = _mm256_add_epi32(r0, _mm256_mullo_epi32(val00v, col0v));
                r0 = _mm256_add_epi32(r0, _mm256_mullo_epi32(val01v, col1v));
                r0 = _mm256_add_epi32(r0, _mm256_mullo_epi32(val02v, col2v));
                r0 = _mm256_add_epi32(r0, _mm256_mullo_epi32(val03v, col3v));
                r0 = _mm256_add_epi32(r0, _mm256_mullo_epi32(val04v, col4v));
                r0 = _mm256_add_epi32(r0, _mm256_mullo_epi32(val05v, col5v));
                r0 = _mm256_add_epi32(r0, _mm256_mullo_epi32(val06v, col6v));
                r0 = _mm256_add_epi32(r0, _mm256_mullo_epi32(val07v, col7v));

                r1 = _mm256_add_epi32(r1, _mm256_mullo_epi32(val10v, col0v));
                r1 = _mm256_add_epi32(r1, _mm256_mullo_epi32(val11v, col1v));
                r1 = _mm256_add_epi32(r1, _mm256_mullo_epi32(val12v, col2v));
                r1 = _mm256_add_epi32(r1, _mm256_mullo_epi32(val13v, col3v));
                r1 = _mm256_add_epi32(r1, _mm256_mullo_epi32(val14v, col4v));
                r1 = _mm256_add_epi32(r1, _mm256_mullo_epi32(val15v, col5v));
                r1 = _mm256_add_epi32(r1, _mm256_mullo_epi32(val16v, col6v));
                r1 = _mm256_add_epi32(r1, _mm256_mullo_epi32(val17v, col7v));

                r2 = _mm256_add_epi32(r2, _mm256_mullo_epi32(val20v, col0v));
                r2 = _mm256_add_epi32(r2, _mm256_mullo_epi32(val21v, col1v));
                r2 = _mm256_add_epi32(r2, _mm256_mullo_epi32(val22v, col2v));
                r2 = _mm256_add_epi32(r2, _mm256_mullo_epi32(val23v, col3v));
                r2 = _mm256_add_epi32(r2, _mm256_mullo_epi32(val24v, col4v));
                r2 = _mm256_add_epi32(r2, _mm256_mullo_epi32(val25v, col5v));
                r2 = _mm256_add_epi32(r2, _mm256_mullo_epi32(val26v, col6v));
                r2 = _mm256_add_epi32(r2, _mm256_mullo_epi32(val27v, col7v));

                r3 = _mm256_add_epi32(r3, _mm256_mullo_epi32(val30v, col0v));
                r3 = _mm256_add_epi32(r3, _mm256_mullo_epi32(val31v, col1v));
                r3 = _mm256_add_epi32(r3, _mm256_mullo_epi32(val32v, col2v));
                r3 = _mm256_add_epi32(r3, _mm256_mullo_epi32(val33v, col3v));
                r3 = _mm256_add_epi32(r3, _mm256_mullo_epi32(val34v, col4v));
                r3 = _mm256_add_epi32(r3, _mm256_mullo_epi32(val35v, col5v));
                r3 = _mm256_add_epi32(r3, _mm256_mullo_epi32(val36v, col6v));
                r3 = _mm256_add_epi32(r3, _mm256_mullo_epi32(val37v, col7v));

                r4 = _mm256_add_epi32(r4, _mm256_mullo_epi32(val40v, col0v));
                r4 = _mm256_add_epi32(r4, _mm256_mullo_epi32(val41v, col1v));
                r4 = _mm256_add_epi32(r4, _mm256_mullo_epi32(val42v, col2v));
                r4 = _mm256_add_epi32(r4, _mm256_mullo_epi32(val43v, col3v));
                r4 = _mm256_add_epi32(r4, _mm256_mullo_epi32(val44v, col4v));
                r4 = _mm256_add_epi32(r4, _mm256_mullo_epi32(val45v, col5v));
                r4 = _mm256_add_epi32(r4, _mm256_mullo_epi32(val46v, col6v));
                r4 = _mm256_add_epi32(r4, _mm256_mullo_epi32(val47v, col7v));

                r5 = _mm256_add_epi32(r5, _mm256_mullo_epi32(val50v, col0v));
                r5 = _mm256_add_epi32(r5, _mm256_mullo_epi32(val51v, col1v));
                r5 = _mm256_add_epi32(r5, _mm256_mullo_epi32(val52v, col2v));
                r5 = _mm256_add_epi32(r5, _mm256_mullo_epi32(val53v, col3v));
                r5 = _mm256_add_epi32(r5, _mm256_mullo_epi32(val54v, col4v));
                r5 = _mm256_add_epi32(r5, _mm256_mullo_epi32(val55v, col5v));
                r5 = _mm256_add_epi32(r5, _mm256_mullo_epi32(val56v, col6v));
                r5 = _mm256_add_epi32(r5, _mm256_mullo_epi32(val57v, col7v));

                r6 = _mm256_add_epi32(r6, _mm256_mullo_epi32(val60v, col0v));
                r6 = _mm256_add_epi32(r6, _mm256_mullo_epi32(val61v, col1v));
                r6 = _mm256_add_epi32(r6, _mm256_mullo_epi32(val62v, col2v));
                r6 = _mm256_add_epi32(r6, _mm256_mullo_epi32(val63v, col3v));
                r6 = _mm256_add_epi32(r6, _mm256_mullo_epi32(val64v, col4v));
                r6 = _mm256_add_epi32(r6, _mm256_mullo_epi32(val65v, col5v));
                r6 = _mm256_add_epi32(r6, _mm256_mullo_epi32(val66v, col6v));
                r6 = _mm256_add_epi32(r6, _mm256_mullo_epi32(val67v, col7v));

                r7 = _mm256_add_epi32(r7, _mm256_mullo_epi32(val70v, col0v));
                r7 = _mm256_add_epi32(r7, _mm256_mullo_epi32(val71v, col1v));
                r7 = _mm256_add_epi32(r7, _mm256_mullo_epi32(val72v, col2v));
                r7 = _mm256_add_epi32(r7, _mm256_mullo_epi32(val73v, col3v));
                r7 = _mm256_add_epi32(r7, _mm256_mullo_epi32(val74v, col4v));
                r7 = _mm256_add_epi32(r7, _mm256_mullo_epi32(val75v, col5v));
                r7 = _mm256_add_epi32(r7, _mm256_mullo_epi32(val76v, col6v));
                r7 = _mm256_add_epi32(r7, _mm256_mullo_epi32(val77v, col7v));

                _mm256_storeu_si256(
                    (__m256i*)&result_row0[j],
                    _mm256_add_epi32(
                        _mm256_loadu_si256((__m256i*)&result_row0[j]), r0));
                _mm256_storeu_si256(
                    (__m256i*)&result_row1[j],
                    _mm256_add_epi32(
                        _mm256_loadu_si256((__m256i*)&result_row1[j]), r1));
                _mm256_storeu_si256(
                    (__m256i*)&result_row2[j],
                    _mm256_add_epi32(
                        _mm256_loadu_si256((__m256i*)&result_row2[j]), r2));
                _mm256_storeu_si256(
                    (__m256i*)&result_row3[j],
                    _mm256_add_epi32(
                        _mm256_loadu_si256((__m256i*)&result_row3[j]), r3));
                _mm256_storeu_si256(
                    (__m256i*)&result_row4[j],
                    _mm256_add_epi32(
                        _mm256_loadu_si256((__m256i*)&result_row4[j]), r4));
                _mm256_storeu_si256(
                    (__m256i*)&result_row5[j],
                    _mm256_add_epi32(
                        _mm256_loadu_si256((__m256i*)&result_row5[j]), r5));
                _mm256_storeu_si256(
                    (__m256i*)&result_row6[j],
                    _mm256_add_epi32(
                        _mm256_loadu_si256((__m256i*)&result_row6[j]), r6));
                _mm256_storeu_si256(
                    (__m256i*)&result_row7[j],
                    _mm256_add_epi32(
                        _mm256_loadu_si256((__m256i*)&result_row7[j]), r7));
            }
        }
    }
    // bottom-left block
    if (m_over || n_over || k_over)
        printf("m_over: %d, n_over: %d, k_over: %d\n", m_over, n_over, k_over);
    {
#pragma omp parallel for schedule(auto)
        for (size_t i = m_tile + local_m_start; i < local_m_end; ++i)
        {
            for (size_t k = 0; k < K; ++k)
            {
                for (size_t j = 0; j < n_tile; ++j)
                {
#pragma omp critical
                    result[i - local_m_start][j] +=
                        matrix1[i][k] * matrix2[k][j];
                }
            }
        }
// top-right block
#pragma omp parallel for schedule(auto)
        for (size_t i = local_m_start; i < m_tile + local_m_start; ++i)
        {
            for (size_t k = 0; k < K; ++k)
            {
                for (size_t j = n_tile; j < N; ++j)
                {
#pragma omp critical
                    result[i - local_m_start][j] +=
                        matrix1[i][k] * matrix2[k][j];
                }
            }
        }
// bottom-right block
#pragma omp parallel for schedule(auto)
        for (size_t i = m_tile + local_m_start; i < local_m_end; ++i)
        {
            for (size_t k = 0; k < K; ++k)
            {
                for (size_t j = n_tile; j < N; ++j)
                {
#pragma omp critical
                    result[i - local_m_start][j] +=
                        matrix1[i][k] * matrix2[k][j];
                }
            }
        }
    }
    return result;
}

int* generateArray(int M, int n)
{
    int* arr = new int[n];
    for (int i = 0; i < n; i++)
    {
        arr[i] = i * M / (n - 1);
        arr[i] = (arr[i] + 4) & (~7);
    }
    return arr;
}

int main(int argc, char** argv)
{
    // Verify input argument format
    if (argc != 5)
    {
        throw std::invalid_argument(
            "Invalid argument, should be: ./executable thread_num "
            "/path/to/matrix1 /path/to/matrix2 /path/to/multiply_result\n");
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
    MPI_Request request;

    int thread_num = atoi(argv[1]);
    omp_set_num_threads(thread_num);

    // Read Matrix
    const std::string matrix1_path = argv[2];

    const std::string matrix2_path = argv[3];

    const std::string result_path = argv[4];

    Matrix matrix1 = Matrix::loadFromFile(matrix1_path);

    Matrix matrix2 = Matrix::loadFromFile(matrix2_path);

    auto M = matrix1.getRows(), K = matrix1.getCols(), N = matrix2.getCols();

    // First several threads are used for tiled matrices multiplication, the
    // last thread is used for dealing with edge cases
    int cut_size = numtasks + 1;
    int* m_cuts = generateArray(M, cut_size);
    // int* n_cuts = generateArray(N, numtasks + 1);

    auto start_time = std::chrono::high_resolution_clock::now();
    if (taskid == MASTER)
    {
        size_t local_m_start = m_cuts[MASTER];   // 0
        size_t local_m_end = m_cuts[MASTER + 1]; // 255
        // local_n = n_cuts[taskid + 1];
        Matrix local_result =
            matrix_multiply_mpi(matrix1, matrix2, local_m_start, local_m_end);
        // printf("master after mat_multiply\n");
        Matrix result(M, N);
        // Your Code Here for Synchronization!
        for (auto row_start = local_m_start, row_end = local_m_end;
             row_start < row_end; ++row_start)
        {
            memcpy(result[row_start], local_result[row_start],
                   N * sizeof(int));
        }
        // printf("local_result[0][256]: %d\n", local_result[0][0]);
        auto row_buffer = new int[N];
        for (int i = MASTER + 1; i < numtasks; ++i)
        {
            for (int row_start = m_cuts[i], row_end = m_cuts[i + 1];
                 row_start < row_end; ++row_start)
            {
                // MPI_Irecv(row_buffer, N, MPI_INT, i, 0, MPI_COMM_WORLD,
                // &request);
                MPI_Recv(row_buffer, N, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
                // MPI_Wait(&request, &status);
                memcpy(result[row_start], row_buffer, N * sizeof(int));
            }
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                                  start_time);

        result.saveToFile(result_path);
        delete[] row_buffer;
        std::cout << "Output file to: " << result_path << std::endl;

        std::cout << "Multiplication Complete!" << std::endl;
        std::cout << "Execution Time: " << elapsed_time.count()
                  << " milliseconds" << std::endl;
    }
    else
    {
        size_t local_m_start = m_cuts[taskid];
        size_t local_m_end = m_cuts[taskid + 1];
        Matrix result =
            matrix_multiply_mpi(matrix1, matrix2, local_m_start, local_m_end);
        // Your Code Here for Synchronization!

        for (int row_start = 0, row_end = local_m_end - local_m_start;
             row_start < row_end; ++row_start)
        {
            // MPI_Isend(result[row_start], N, MPI_INT, MASTER, 0,
            // MPI_COMM_WORLD, &request);
            MPI_Send(result[row_start], N, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
            // MPI_Wait(&request, &status);
        }
    }

    MPI_Finalize();
    delete[] m_cuts;
    // delete[] n_cuts;
    return 0;
}
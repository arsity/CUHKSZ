//
// Created by Yang Yufan on 2023/10/07.
// Email: yufanyang1@link.cuhk.edu.cn
//
// Reordering Matrix Multiplication
//

#include <stdexcept>
#include <chrono>
#include "matrix.hpp"

Matrix matrix_multiply_locality(const Matrix& matrix1, const Matrix& matrix2)
{
    if (matrix1.getCols() != matrix2.getRows())
    {
        throw std::invalid_argument(
            "Matrix dimensions are not compatible for multiplication.");
    }
    // Matrix 1: [M x K], Matrix 2: [K x N]
    size_t M = matrix1.getRows(), K = matrix1.getCols(), N = matrix2.getCols();
    // Result Matrix: [M x N] (i, j)
    Matrix result(M, N);

    // Your Code Here!
    // Optimizing Matrix Multiplication
    // Considering Memory Locality and Avoiding Cache Missing
    // Hints:
    // 1. Change the order of the triple nested loop
    // 2. Apply Tiled Matrix Multiplication

    for (size_t i = 0; i < M; i += 4)
    {
        auto a_row0 = matrix1[i];
        auto a_row1 = matrix1[i + 1];
        auto a_row2 = matrix1[i + 2];
        auto a_row3 = matrix1[i + 3];

        auto result_row0 = result[i];
        auto result_row1 = result[i + 1];
        auto result_row2 = result[i + 2];
        auto result_row3 = result[i + 3];

        for (size_t k = 0; k < K; k += 4)
        {
            auto b_row0 = matrix2[k];
            auto b_row1 = matrix2[k + 1];
            auto b_row2 = matrix2[k + 2];
            auto b_row3 = matrix2[k + 3];

            auto a_val00 = a_row0[k];
            auto a_val01 = a_row0[k + 1];
            auto a_val02 = a_row0[k + 2];
            auto a_val03 = a_row0[k + 3];

            auto a_val10 = a_row1[k];
            auto a_val11 = a_row1[k + 1];
            auto a_val12 = a_row1[k + 2];
            auto a_val13 = a_row1[k + 3];

            auto a_val20 = a_row2[k];
            auto a_val21 = a_row2[k + 1];
            auto a_val22 = a_row2[k + 2];
            auto a_val23 = a_row2[k + 3];

            auto a_val30 = a_row3[k];
            auto a_val31 = a_row3[k + 1];
            auto a_val32 = a_row3[k + 2];
            auto a_val33 = a_row3[k + 3];

            for (size_t j = 0; j < N; j += 4)
            {
                result_row0[j] += a_val00 * b_row0[j] + a_val01 * b_row1[j] +
                                  a_val02 * b_row2[j] + a_val03 * b_row3[j];
                result_row0[j + 1] +=
                    a_val00 * b_row0[j + 1] + a_val01 * b_row1[j + 1] +
                    a_val02 * b_row2[j + 1] + a_val03 * b_row3[j + 1];
                result_row0[j + 2] +=
                    a_val00 * b_row0[j + 2] + a_val01 * b_row1[j + 2] +
                    a_val02 * b_row2[j + 2] + a_val03 * b_row3[j + 2];
                result_row0[j + 3] +=
                    a_val00 * b_row0[j + 3] + a_val01 * b_row1[j + 3] +
                    a_val02 * b_row2[j + 3] + a_val03 * b_row3[j + 3];

                result_row1[j] += a_val10 * b_row0[j] + a_val11 * b_row1[j] +
                                  a_val12 * b_row2[j] + a_val13 * b_row3[j];
                result_row1[j + 1] +=
                    a_val10 * b_row0[j + 1] + a_val11 * b_row1[j + 1] +
                    a_val12 * b_row2[j + 1] + a_val13 * b_row3[j + 1];
                result_row1[j + 2] +=
                    a_val10 * b_row0[j + 2] + a_val11 * b_row1[j + 2] +
                    a_val12 * b_row2[j + 2] + a_val13 * b_row3[j + 2];
                result_row1[j + 3] +=
                    a_val10 * b_row0[j + 3] + a_val11 * b_row1[j + 3] +
                    a_val12 * b_row2[j + 3] + a_val13 * b_row3[j + 3];

                result_row2[j] += a_val20 * b_row0[j] + a_val21 * b_row1[j] +
                                  a_val22 * b_row2[j] + a_val23 * b_row3[j];
                result_row2[j + 1] +=
                    a_val20 * b_row0[j + 1] + a_val21 * b_row1[j + 1] +
                    a_val22 * b_row2[j + 1] + a_val23 * b_row3[j + 1];
                result_row2[j + 2] +=
                    a_val20 * b_row0[j + 2] + a_val21 * b_row1[j + 2] +
                    a_val22 * b_row2[j + 2] + a_val23 * b_row3[j + 2];
                result_row2[j + 3] +=
                    a_val20 * b_row0[j + 3] + a_val21 * b_row1[j + 3] +
                    a_val22 * b_row2[j + 3] + a_val23 * b_row3[j + 3];

                result_row3[j] += a_val30 * b_row0[j] + a_val31 * b_row1[j] +
                                  a_val32 * b_row2[j] + a_val33 * b_row3[j];
                result_row3[j + 1] +=
                    a_val30 * b_row0[j + 1] + a_val31 * b_row1[j + 1] +
                    a_val32 * b_row2[j + 1] + a_val33 * b_row3[j + 1];
                result_row3[j + 2] +=
                    a_val30 * b_row0[j + 2] + a_val31 * b_row1[j + 2] +
                    a_val32 * b_row2[j + 2] + a_val33 * b_row3[j + 2];
                result_row3[j + 3] +=
                    a_val30 * b_row0[j + 3] + a_val31 * b_row1[j + 3] +
                    a_val32 * b_row2[j + 3] + a_val33 * b_row3[j + 3];
            }
        }
    }

    return result;
}

int main(int argc, char** argv)
{
    // Verify input argument format
    if (argc != 4)
    {
        throw std::invalid_argument(
            "Invalid argument, should be: ./executable "
            "/path/to/matrix1 /path/to/matrix2 /path/to/multiply_result\n");
    }

    const std::string matrix1_path = argv[1];

    const std::string matrix2_path = argv[2];

    const std::string result_path = argv[3];

    Matrix matrix1 = Matrix::loadFromFile(matrix1_path);

    Matrix matrix2 = Matrix::loadFromFile(matrix2_path);

    auto start_time = std::chrono::high_resolution_clock::now();

    Matrix result = matrix_multiply_locality(matrix1, matrix2);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);

    result.saveToFile(result_path);

    std::cout << "Output file to: " << result_path << std::endl;

    std::cout << "Multiplication Complete!" << std::endl;
    std::cout << "Execution Time: " << elapsed_time.count() << " milliseconds"
              << std::endl;

    return 0;
}

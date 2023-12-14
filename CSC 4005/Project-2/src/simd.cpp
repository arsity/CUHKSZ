//
// Created by Yang Yufan on 2023/10/07.
// Email: yufanyang1@link.cuhk.edu.cn
//
// SIMD + Reordering Matrix Multiplication
//

#include <immintrin.h>
#include <stdexcept>
#include <chrono>
#include "matrix.hpp"

Matrix matrix_multiply_simd(const Matrix& matrix1, const Matrix& matrix2)
{
    if (matrix1.getCols() != matrix2.getRows())
    {
        throw std::invalid_argument(
            "Matrix dimensions are not compatible for multiplication.");
    }

    size_t M = matrix1.getRows(), K = matrix1.getCols(), N = matrix2.getCols();

    Matrix result(M, N);

    // Your Code Here!
    // Optimizing Matrix Multiplication
    // In addition to Memory Locality and Cache Missing,
    // Further Applying SIMD

    for (size_t i = 0; i < M; i += 8)
    {
        auto a_row0 = matrix1[i];
        auto a_row1 = matrix1[i + 1];
        auto a_row2 = matrix1[i + 2];
        auto a_row3 = matrix1[i + 3];
        auto a_row4 = matrix1[i + 4];
        auto a_row5 = matrix1[i + 5];
        auto a_row6 = matrix1[i + 6];
        auto a_row7 = matrix1[i + 7];

        auto result_row0 = result[i];
        auto result_row1 = result[i + 1];
        auto result_row2 = result[i + 2];
        auto result_row3 = result[i + 3];
        auto result_row4 = result[i + 4];
        auto result_row5 = result[i + 5];
        auto result_row6 = result[i + 6];
        auto result_row7 = result[i + 7];

        for (size_t k = 0; k < K; k += 8)
        {
            auto b_row0 = matrix2[k];
            auto b_row1 = matrix2[k + 1];
            auto b_row2 = matrix2[k + 2];
            auto b_row3 = matrix2[k + 3];
            auto b_row4 = matrix2[k + 4];
            auto b_row5 = matrix2[k + 5];
            auto b_row6 = matrix2[k + 6];
            auto b_row7 = matrix2[k + 7];

            // auto val0 = _mm256_loadu_si256((__m256i*)&row0[k]); // val00 -
            // val07
            auto val00v = _mm256_set1_epi32(a_row0[k]);
            auto val01v = _mm256_set1_epi32(a_row0[k + 1]);
            auto val02v = _mm256_set1_epi32(a_row0[k + 2]);
            auto val03v = _mm256_set1_epi32(a_row0[k + 3]);
            auto val04v = _mm256_set1_epi32(a_row0[k + 4]);
            auto val05v = _mm256_set1_epi32(a_row0[k + 5]);
            auto val06v = _mm256_set1_epi32(a_row0[k + 6]);
            auto val07v = _mm256_set1_epi32(a_row0[k + 7]);

            // auto val1 = _mm256_loadu_si256((__m256i*)&row1[k]); // val10 -
            // val17
            auto val10v = _mm256_set1_epi32(a_row1[k]);
            auto val11v = _mm256_set1_epi32(a_row1[k + 1]);
            auto val12v = _mm256_set1_epi32(a_row1[k + 2]);
            auto val13v = _mm256_set1_epi32(a_row1[k + 3]);
            auto val14v = _mm256_set1_epi32(a_row1[k + 4]);
            auto val15v = _mm256_set1_epi32(a_row1[k + 5]);
            auto val16v = _mm256_set1_epi32(a_row1[k + 6]);
            auto val17v = _mm256_set1_epi32(a_row1[k + 7]);

            // auto val2 = _mm256_loadu_si256((__m256i*)&row2[k]); // val20 -
            // val27
            auto val20v = _mm256_set1_epi32(a_row2[k]);
            auto val21v = _mm256_set1_epi32(a_row2[k + 1]);
            auto val22v = _mm256_set1_epi32(a_row2[k + 2]);
            auto val23v = _mm256_set1_epi32(a_row2[k + 3]);
            auto val24v = _mm256_set1_epi32(a_row2[k + 4]);
            auto val25v = _mm256_set1_epi32(a_row2[k + 5]);
            auto val26v = _mm256_set1_epi32(a_row2[k + 6]);
            auto val27v = _mm256_set1_epi32(a_row2[k + 7]);

            // auto val3 = _mm256_loadu_si256((__m256i*)&row3[k]); // val30 -
            // val37
            auto val30v = _mm256_set1_epi32(a_row3[k]);
            auto val31v = _mm256_set1_epi32(a_row3[k + 1]);
            auto val32v = _mm256_set1_epi32(a_row3[k + 2]);
            auto val33v = _mm256_set1_epi32(a_row3[k + 3]);
            auto val34v = _mm256_set1_epi32(a_row3[k + 4]);
            auto val35v = _mm256_set1_epi32(a_row3[k + 5]);
            auto val36v = _mm256_set1_epi32(a_row3[k + 6]);
            auto val37v = _mm256_set1_epi32(a_row3[k + 7]);

            // val40
            auto val40v = _mm256_set1_epi32(a_row4[k]);
            auto val41v = _mm256_set1_epi32(a_row4[k + 1]);
            auto val42v = _mm256_set1_epi32(a_row4[k + 2]);
            auto val43v = _mm256_set1_epi32(a_row4[k + 3]);
            auto val44v = _mm256_set1_epi32(a_row4[k + 4]);
            auto val45v = _mm256_set1_epi32(a_row4[k + 5]);
            auto val46v = _mm256_set1_epi32(a_row4[k + 6]);
            auto val47v = _mm256_set1_epi32(a_row4[k + 7]);

            // val50
            auto val50v = _mm256_set1_epi32(a_row5[k]);
            auto val51v = _mm256_set1_epi32(a_row5[k + 1]);
            auto val52v = _mm256_set1_epi32(a_row5[k + 2]);
            auto val53v = _mm256_set1_epi32(a_row5[k + 3]);
            auto val54v = _mm256_set1_epi32(a_row5[k + 4]);
            auto val55v = _mm256_set1_epi32(a_row5[k + 5]);
            auto val56v = _mm256_set1_epi32(a_row5[k + 6]);
            auto val57v = _mm256_set1_epi32(a_row5[k + 7]);

            // val60
            auto val60v = _mm256_set1_epi32(a_row6[k]);
            auto val61v = _mm256_set1_epi32(a_row6[k + 1]);
            auto val62v = _mm256_set1_epi32(a_row6[k + 2]);
            auto val63v = _mm256_set1_epi32(a_row6[k + 3]);
            auto val64v = _mm256_set1_epi32(a_row6[k + 4]);
            auto val65v = _mm256_set1_epi32(a_row6[k + 5]);
            auto val66v = _mm256_set1_epi32(a_row6[k + 6]);
            auto val67v = _mm256_set1_epi32(a_row6[k + 7]);

            // val70
            auto val70v = _mm256_set1_epi32(a_row7[k]);
            auto val71v = _mm256_set1_epi32(a_row7[k + 1]);
            auto val72v = _mm256_set1_epi32(a_row7[k + 2]);
            auto val73v = _mm256_set1_epi32(a_row7[k + 3]);
            auto val74v = _mm256_set1_epi32(a_row7[k + 4]);
            auto val75v = _mm256_set1_epi32(a_row7[k + 5]);
            auto val76v = _mm256_set1_epi32(a_row7[k + 6]);
            auto val77v = _mm256_set1_epi32(a_row7[k + 7]);

            for (size_t j = 0; j < N; j += 8)
            {
                // Declare 7 256bit vectors (8 int32)
                auto r0 = _mm256_setzero_si256();
                auto r1 = _mm256_setzero_si256();
                auto r2 = _mm256_setzero_si256();
                auto r3 = _mm256_setzero_si256();
                auto r4 = _mm256_setzero_si256();
                auto r5 = _mm256_setzero_si256();
                auto r6 = _mm256_setzero_si256();
                auto r7 = _mm256_setzero_si256();

                auto col0v = _mm256_loadu_si256((__m256i*)&b_row0[j]);
                auto col1v = _mm256_loadu_si256((__m256i*)&b_row1[j]);
                auto col2v = _mm256_loadu_si256((__m256i*)&b_row2[j]);
                auto col3v = _mm256_loadu_si256((__m256i*)&b_row3[j]);
                auto col4v = _mm256_loadu_si256((__m256i*)&b_row4[j]);
                auto col5v = _mm256_loadu_si256((__m256i*)&b_row5[j]);
                auto col6v = _mm256_loadu_si256((__m256i*)&b_row6[j]);
                auto col7v = _mm256_loadu_si256((__m256i*)&b_row7[j]);

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

    Matrix result = matrix_multiply_simd(matrix1, matrix2);

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
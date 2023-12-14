#include <iostream>
#include <cmath>
#include <chrono>
#include <immintrin.h> // for AVX2 intrinsics

#include "utils.hpp"

const int FILTER_SIZE = 3;
const float filter[FILTER_SIZE][FILTER_SIZE] = {{1.0 / 9, 1.0 / 9, 1.0 / 9},
                                                {1.0 / 9, 1.0 / 9, 1.0 / 9},
                                                {1.0 / 9, 1.0 / 9, 1.0 / 9}};

int main(int argc, char** argv)
{
    // Verify input argument format
    if (argc != 3)
    {
        std::cerr << "Invalid argument, should be: ./executable "
                     "/path/to/input/jpeg /path/to/output/jpeg\n";
        return -1;
    }
    // Read JPEG File
    const char* input_filepath = argv[1];
    std::cout << "Input file from: " << input_filepath << "\n";
    auto input_jpeg = read_from_jpeg(input_filepath);
    if (input_jpeg.buffer == NULL)
    {
        std::cerr << "Failed to read input JPEG image\n";
        return -1;
    }

    // Store the smoothed image
    auto filteredImage =
        new unsigned char[input_jpeg.width * input_jpeg.height *
                              input_jpeg.num_channels +
                          8];

    auto redImage =
        new unsigned char[input_jpeg.width * input_jpeg.height + 16];
    auto greenImage =
        new unsigned char[input_jpeg.width * input_jpeg.height + 16];
    auto blueImage =
        new unsigned char[input_jpeg.width * input_jpeg.height + 16];
    // Prepross, store reds, greens and blues separately
    auto reds = new unsigned char[input_jpeg.width * input_jpeg.height + 16];
    auto greens = new unsigned char[input_jpeg.width * input_jpeg.height + 16];
    auto blues = new unsigned char[input_jpeg.width * input_jpeg.height + 16];

    for (int i = 0; i < input_jpeg.width * input_jpeg.height; i++)
    {
        reds[i] = input_jpeg.buffer[i * input_jpeg.num_channels];
        greens[i] = input_jpeg.buffer[i * input_jpeg.num_channels + 1];
        blues[i] = input_jpeg.buffer[i * input_jpeg.num_channels + 2];
    }

    // Mask used for shuffling when store int32s to u_int8 arrays
    // |0|0|0|4|0|0|0|3|0|0|0|2|0|0|0|1| -> |4|3|2|1|
    __m128i shuffle = _mm_setr_epi8(0, 4, 8, 12, -1, -1, -1, -1, -1, -1, -1, -1,
                                    -1, -1, -1, -1);

    auto start_time =
        std::chrono::high_resolution_clock::now(); // Start recording time
#pragma unroll(32)
    for (int height = 1; height < input_jpeg.height - 1; ++height)
    {
#pragma unroll(32)
        for (int width = 1; width < input_jpeg.width - 1; width += 8)
        {
            int idx = height * input_jpeg.width + width;

            auto r_sum = _mm256_setzero_ps();
            auto g_sum = _mm256_setzero_ps();
            auto b_sum = _mm256_setzero_ps();

#pragma unroll(FILTER_SIZE)
            for (int i = -1; i <= 1; ++i)
            {
#pragma unroll(FILTER_SIZE)
                for (int j = -1; j <= 1; ++j)
                {
                    // Load the filter scalar
                    __m256 filterScalar = _mm256_set1_ps(filter[i + 1][j + 1]);
                    // Load the 8 red chars to a 256 bits float register
                    __m128i red_chars = _mm_loadu_si128(
                        (__m128i*)(reds + idx + i * input_jpeg.width + j));
                    __m256i red_ints = _mm256_cvtepu8_epi32(red_chars);
                    __m256 red_floats = _mm256_cvtepi32_ps(red_ints);
                    // Load the 8 green chars to a 256 bits float register
                    __m128i green_chars = _mm_loadu_si128(
                        (__m128i*)(greens + idx + i * input_jpeg.width + j));
                    __m256i green_ints = _mm256_cvtepu8_epi32(green_chars);
                    __m256 green_floats = _mm256_cvtepi32_ps(green_ints);
                    // Load the 8 blue chars to a 256 bits float register
                    __m128i blue_chars = _mm_loadu_si128(
                        (__m128i*)(blues + idx + i * input_jpeg.width + j));
                    __m256i blue_ints = _mm256_cvtepu8_epi32(blue_chars);
                    __m256 blue_floats = _mm256_cvtepi32_ps(blue_ints);

                    // Multiply the three channels to the filter scalar
                    __m256 red_results =
                        _mm256_mul_ps(red_floats, filterScalar);
                    __m256 green_results =
                        _mm256_mul_ps(green_floats, filterScalar);
                    __m256 blue_results =
                        _mm256_mul_ps(blue_floats, filterScalar);

                    // Add the new results to the existing data
                    r_sum = _mm256_add_ps(r_sum, red_results);
                    g_sum = _mm256_add_ps(g_sum, green_results);
                    b_sum = _mm256_add_ps(b_sum, blue_results);
                }
            }
            // Convert the float32 results to int32
            __m256i new_r_ints = _mm256_cvtps_epi32(r_sum);
            __m256i new_g_ints = _mm256_cvtps_epi32(g_sum);
            __m256i new_b_ints = _mm256_cvtps_epi32(b_sum);

            // Seperate the 256bits result to 2 128bits result
            __m128i low_r = _mm256_castsi256_si128(new_r_ints);
            __m128i high_r = _mm256_extracti128_si256(new_r_ints, 1);
            __m128i low_g = _mm256_castsi256_si128(new_g_ints);
            __m128i high_g = _mm256_extracti128_si256(new_g_ints, 1);
            __m128i low_b = _mm256_castsi256_si128(new_b_ints);
            __m128i high_b = _mm256_extracti128_si256(new_b_ints, 1);

            // Shuffling int32s to u_int8s
            // |0|0|0|4|0|0|0|3|0|0|0|2|0|0|0|1| -> |4|3|2|1|
            __m128i r_trans_low = _mm_shuffle_epi8(low_r, shuffle);
            __m128i r_trans_high = _mm_shuffle_epi8(high_r, shuffle);

            __m128i g_trans_low = _mm_shuffle_epi8(low_g, shuffle);
            __m128i g_trans_high = _mm_shuffle_epi8(high_g, shuffle);

            __m128i b_trans_low = _mm_shuffle_epi8(low_b, shuffle);
            __m128i b_trans_high = _mm_shuffle_epi8(high_b, shuffle);

            // Store the results back to filteredImage
            _mm_storeu_si128((__m128i*)(&redImage[idx]), r_trans_low);
            _mm_storeu_si128((__m128i*)(&redImage[idx + 4]), r_trans_high);
            _mm_storeu_si128((__m128i*)(&greenImage[idx]), g_trans_low);
            _mm_storeu_si128((__m128i*)(&greenImage[idx + 4]), g_trans_high);
            _mm_storeu_si128((__m128i*)(&blueImage[idx]), b_trans_low);
            _mm_storeu_si128((__m128i*)(&blueImage[idx + 4]), b_trans_high);
        }
    }

    for (int i = 0; i < input_jpeg.width * input_jpeg.height; i++)
    {
        filteredImage[i * input_jpeg.num_channels] = redImage[i];
        filteredImage[i * input_jpeg.num_channels + 1] = greenImage[i];
        filteredImage[i * input_jpeg.num_channels + 2] = blueImage[i];
    }

    auto end_time =
        std::chrono::high_resolution_clock::now(); // Stop recording time
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);

    // Save output Gray JPEG Image
    const char* output_filepath = argv[2];
    std::cout << "Output file to: " << output_filepath << "\n";
    JPEGMeta output_jpeg{filteredImage, input_jpeg.width, input_jpeg.height,
                         input_jpeg.num_channels, input_jpeg.color_space};
    if (write_to_jpeg(output_jpeg, output_filepath))
    {
        std::cerr << "Failed to write output JPEG\n";
        return -1;
    }

    // Release allocated memory
    delete[] input_jpeg.buffer;
    delete[] filteredImage;
    delete[] redImage;
    delete[] greenImage;
    delete[] blueImage;
    delete[] reds;
    delete[] greens;
    delete[] blues;
    std::cout << "Smoothing Complete!" << std::endl;
    std::cout << "Execution Time: " << elapsed_time.count()
              << " milliseconds\n";
    return 0;
}

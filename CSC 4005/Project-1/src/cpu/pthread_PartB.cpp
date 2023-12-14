#include <iostream>
#include <cmath>
#include <chrono>
#include <pthread.h>
#include <cstring>
#include "utils.hpp"
#include <numeric>

const int FILTER_SIZE = 3;
const float filter[FILTER_SIZE][FILTER_SIZE] = {{1.0 / 9, 1.0 / 9, 1.0 / 9},
                                                {1.0 / 9, 1.0 / 9, 1.0 / 9},
                                                {1.0 / 9, 1.0 / 9, 1.0 / 9}};
constexpr float one_ninth = 1.0 / 9.0;

// Structure to pass data to each thread
struct ThreadData
{
    unsigned char* input_buffer;
    unsigned char* output_buffer;
    // Notify target position range
    size_t start;
    size_t end;
    // Notify the picture edge
    size_t width;
    size_t height;
};

// Function to convert RGB to Grayscale for a portion of the image
void* conv(void* arg)
{
    auto data = reinterpret_cast<ThreadData*>(arg);
#pragma unroll(32)
    for (auto pos = data->start; pos < data->end; pos++)
    {
#pragma unroll(32)
        for (auto color = 0; color < 3; color++)
        {
            int buffer[9] = {};
#pragma unroll(FILTER_SIZE)
            for (auto i = -1; i < 2; i++)
            {
#pragma unroll(FILTER_SIZE)
                for (auto j = -1; j < 2; j++)
                {
                    buffer[(i + 1) * 3 + j + 1] =
                        data->input_buffer[(pos + i * data->width + j) * 3 +
                                           color] *
                        filter[i + 1][j + 1];
                }
            }
            data->output_buffer[pos * 3 + color] = static_cast<unsigned char>(
                std::accumulate(buffer, buffer + 9, 0.0));
        }
    }

    return nullptr;
}

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cerr << "Invalid argument, should be: ./executable "
                     "/path/to/input/jpeg /path/to/output/jpeg\n";
        return -1;
    }

    size_t num_threads = std::stoi(argv[3]); // User-specified thread count

    // Read input JPEG image
    const char* input_filename = argv[1];
    std::cout << "Input file from: " << input_filename << "\n";
    auto input_jpeg = read_from_jpeg(input_filename);

    // Apply the filter to the image
    auto filteredImage =
        new unsigned char[input_jpeg.width * input_jpeg.height *
                          input_jpeg.num_channels];
    memset(filteredImage, 0,
           input_jpeg.width * input_jpeg.height * input_jpeg.num_channels *
               sizeof(unsigned char));

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    auto chunk_size =
        (input_jpeg.width - 2) * (input_jpeg.height - 2) / num_threads;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].input_buffer = input_jpeg.buffer;
        thread_data[i].output_buffer = filteredImage;
        thread_data[i].start = input_jpeg.width + 2 + i * chunk_size;
        thread_data[i].end =
            (i == num_threads - 1)
                ? (input_jpeg.width - 2) * (input_jpeg.height - 2)
                : input_jpeg.width + 2 + (i + 1) * chunk_size;
        thread_data[i].width = input_jpeg.width;
        thread_data[i].height = input_jpeg.height;

        pthread_create(&threads[i], nullptr, conv, &thread_data[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], nullptr);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);
    // Save output JPEG image
    const char* output_filepath = argv[2];
    std::cout << "Output file to: " << output_filepath << "\n";
    JPEGMeta output_jpeg{filteredImage, input_jpeg.width, input_jpeg.height,
                         input_jpeg.num_channels, input_jpeg.color_space};
    if (write_to_jpeg(output_jpeg, output_filepath))
    {
        std::cerr << "Failed to write output JPEG\n";
        return -1;
    }
    // Post-processing
    delete[] input_jpeg.buffer;
    delete[] filteredImage;
    std::cout << "Transformation Complete!" << std::endl;
    std::cout << "Execution Time: " << elapsed_time.count()
              << " milliseconds\n";
    return 0;
}

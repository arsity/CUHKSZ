#include <iostream>
#include <cmath>
#include <chrono>
#include <cstring>
#include <numeric>
#include "utils.hpp"

const int FILTER_SIZE = 3;
const float filter[FILTER_SIZE][FILTER_SIZE] = {{1.0 / 9, 1.0 / 9, 1.0 / 9},
                                                {1.0 / 9, 1.0 / 9, 1.0 / 9},
                                                {1.0 / 9, 1.0 / 9, 1.0 / 9}};

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cerr << "Invalid argument, should be: ./executable "
                     "/path/to/input/jpeg /path/to/output/jpeg\n";
        return -1;
    }
    auto num_threads = std::stoi(argv[3]);
    // Read input JPEG image
    const char* input_filename = argv[1];
    std::cout << "Input file from: " << input_filename << "\n";
    auto input_jpeg = read_from_jpeg(input_filename);
    size_t width = input_jpeg.width;
    size_t height = input_jpeg.height;
    size_t num_channels = input_jpeg.num_channels;
    auto source = new unsigned char[width * height * num_channels];
    memcpy(source, input_jpeg.buffer,
           width * height * num_channels * sizeof(unsigned char));
    // Apply the filter to the image
    auto filteredImage = new unsigned char[width * height * num_channels];
    memset(filteredImage, 255,
           input_jpeg.width * input_jpeg.height * input_jpeg.num_channels *
               sizeof(unsigned char));

#pragma acc enter data copyin(                        \
    filteredImage[0 : width * height * num_channels], \
    source[0 : width * height * num_channels])

#pragma acc update device(filteredImage[0 : width * height * num_channels], \
                          source[0 : width * height * num_channels])

    auto start_time = std::chrono::high_resolution_clock::now();
#pragma acc parallel present(filteredImage[0 : width * height * num_channels], \
                             source[0 : width * height * num_channels])        \
    num_gangs(num_threads)
    {
#pragma acc loop independent
#pragma unroll(32)
        for (auto index = 0; index < width * height * num_channels; index++)
        {
            auto row = index / num_channels / width;
            auto col = index / num_channels % width;
            auto color = index % num_channels;

            if (row == 0 || row == height - 1 || col == 0 || col == width - 1)
            {
                continue;
            }

            int buffer[9] = {};
#pragma unroll(FILTER_SIZE)
            for (auto i = -1; i < 2; i++)
            {
#pragma unroll(FILTER_SIZE)
                for (auto j = -1; j < 2; j++)
                {
                    buffer[(i + 1) * 3 + j + 1] =
                        source[((row + i) * width + (col + j)) * num_channels +
                               color] *
                        filter[i + 1][j + 1];
                }
            }

            filteredImage[index] = static_cast<unsigned char>(
                std::accumulate(buffer, buffer + 9, 0.0));
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
#pragma acc update self(filteredImage[0 : width * height * num_channels], \
                        source[0 : width * height * num_channels])

#pragma acc exit data copyout(filteredImage[0 : width * height * num_channels])

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
    delete[] source;
    std::cout << "Transformation Complete!" << std::endl;
    std::cout << "Execution Time: " << elapsed_time.count()
              << " milliseconds\n";
    return 0;
}

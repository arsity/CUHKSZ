#include <iostream>

#include <cuda_runtime.h> // CUDA Header
#include "utils.hpp"

constexpr int FILTER_SIZE = 3;
constexpr float filter[FILTER_SIZE][FILTER_SIZE] = {
    {1.0 / 9, 1.0 / 9, 1.0 / 9},
    {1.0 / 9, 1.0 / 9, 1.0 / 9},
    {1.0 / 9, 1.0 / 9, 1.0 / 9}};

// CUDA kernel function: 3x3 convolution
__global__ void convolution(const unsigned char* input, unsigned char* output,
                            const float* fl, int width, int height,
                            int num_channels)
{
    auto idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx > width * height * num_channels) return;

    auto row = idx / num_channels / width;
    auto col = idx / num_channels % width;

    if (row == 0 || row == height - 1 || col == 0 || col == width - 1) return;

    auto color = idx % num_channels;
    int buffer[9];

    // Apply the filter to the image
#pragma unroll(FILTER_SIZE)
    for (auto i = 0; i < FILTER_SIZE; i++)
    {
#pragma unroll(FILTER_SIZE)
        for (auto j = 0; j < FILTER_SIZE; j++)
        {
            auto neighbor_row = row + i - 1;
            auto neighbor_col = col + j - 1;
            auto neighbor_color = color;
            auto neighbor_index = neighbor_row * width * num_channels +
                                  neighbor_col * num_channels + neighbor_color;
            auto filter_value = fl[i * FILTER_SIZE + j];
            auto source_value = input[neighbor_index];
            auto result_value = static_cast<int>(filter_value * source_value);
            buffer[i * FILTER_SIZE + j] = result_value;
        }
    }
    output[idx] = static_cast<unsigned char>(buffer[0] + buffer[1] + buffer[2] +
                                             buffer[3] + buffer[4] + buffer[5] +
                                             buffer[6] + buffer[7] + buffer[8]);
}

int main(int argc, char** argv)
{
    // Verify input arguments format
    if (argc != 3)
    {
        std::cerr << "Invalid argument, should be: ./executable "
                     "/path/to/input/jpeg /path/to/output/jpeg\n";
        return -1;
    }
    // Read from input JPEG file
    const char* input_filepath = argv[1];
    std::cout << "Input file from: " << input_filepath << "\n";
    auto input_jpeg = read_from_jpeg(input_filepath);
    // Allocate memory on host (CPU)
    auto filteredImage =
        new unsigned char[input_jpeg.width * input_jpeg.height *
                          input_jpeg.num_channels];
    // Allocate memory on device (GPU)
    unsigned char* d_input;
    unsigned char* d_output;
    float* d_filter;
    cudaMalloc((void**)&d_input, input_jpeg.width * input_jpeg.height *
                                     input_jpeg.num_channels *
                                     sizeof(unsigned char));
    cudaMalloc((void**)&d_output, input_jpeg.width * input_jpeg.height *
                                      input_jpeg.num_channels *
                                      sizeof(unsigned char));
    cudaMalloc((void**)&d_filter, FILTER_SIZE * FILTER_SIZE * sizeof(float));
    // Copy input data from host to device
    cudaMemcpy(d_input, input_jpeg.buffer,
               input_jpeg.width * input_jpeg.height * input_jpeg.num_channels *
                   sizeof(unsigned char),
               cudaMemcpyHostToDevice);
    cudaMemcpy(d_filter, filter, FILTER_SIZE * FILTER_SIZE * sizeof(float),
               cudaMemcpyHostToDevice);
    // Computation: Filter
    cudaEvent_t start, stop;
    float gpuDuration;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    // Calculate the number of blocks and threads
    int blockSize;   // The launch configurator returned block size
    int minGridSize; // The minimum grid size needed to achieve the maximum
                     // occupancy for a full device launch
    int gridSize;    // The actual grid size needed, based on input size
    cudaOccupancyMaxPotentialBlockSize(&minGridSize, &blockSize, convolution);
    gridSize = (input_jpeg.width * input_jpeg.height * input_jpeg.num_channels +
                blockSize - 1) /
               blockSize;

    // GPU start time
    cudaEventRecord(start, 0);
    // Call kernel function
    convolution<<<gridSize, blockSize>>>(d_input, d_output, d_filter,
                                         input_jpeg.width, input_jpeg.height,
                                         input_jpeg.num_channels);
    // GPU stop time
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    // Print the GPU execution time
    cudaEventElapsedTime(&gpuDuration, start, stop);
    // Copy output data from device to host
    cudaMemcpy(filteredImage, d_output,
               input_jpeg.width * input_jpeg.height * input_jpeg.num_channels *
                   sizeof(unsigned char),
               cudaMemcpyDeviceToHost);
    // Write to output JPEG file
    const char* output_filepath = argv[2];
    std::cout << "Output file to: " << output_filepath << "\n";
    JPEGMeta output_jpeg{filteredImage, input_jpeg.width, input_jpeg.height,
                         input_jpeg.num_channels, input_jpeg.color_space};
    if (write_to_jpeg(output_jpeg, output_filepath))
    {
        std::cerr << "Failed to write output JPEG\n";
        return -1;
    }
    // Free memory on host (CPU)
    delete[] filteredImage;
    delete[] input_jpeg.buffer;
    // Free memory on device (GPU)
    cudaFree(d_input);
    cudaFree(d_output);

    std::cout << "Smoothing Complete!" << std::endl;
    std::cout << "GPU Execution Time: " << gpuDuration << " milliseconds"
              << std::endl;
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    return 0;
}
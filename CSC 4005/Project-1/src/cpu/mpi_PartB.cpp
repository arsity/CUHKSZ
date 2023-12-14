#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>

#include <mpi.h> // MPI Header

#include "utils.hpp"

#define MASTER 0
#define TAG_GATHER 0

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
    // Start the MPI
    MPI_Init(&argc, &argv);
    int num_tasks;
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
    int task_id;
    MPI_Comm_rank(MPI_COMM_WORLD, &task_id);
    int len;
    char host_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Get_processor_name(host_name, &len);
    MPI_Status status;

    // Read input JPEG image
    const char* input_filename = argv[1];
    std::cout << "Input file from: " << input_filename << "\n";
    auto input_jpeg = read_from_jpeg(input_filename);

    auto start_time = std::chrono::high_resolution_clock::now();

    // Divide the task
    int row_num_per_task = input_jpeg.height / num_tasks;
    int left_row_num = input_jpeg.height % num_tasks;

    // cuts is the beginning index of each task
    std::vector<int> cuts(num_tasks + 1, 0);
    int divided_left_row_num = 0;

    // Ignore the first row
    cuts.front() = 1;

    for (int i = 0; i < num_tasks; ++i)
    {
        if (divided_left_row_num < left_row_num)
        {
            cuts[i + 1] = cuts[i] + row_num_per_task + 1;
            ++divided_left_row_num;
        }
        else
        {
            cuts[i + 1] = cuts[i] + row_num_per_task;
        }
    }
    // Ignore the last row
    cuts.back() = cuts.back() - 2;

    // The tasks for the master executor
    // 1. Smooth the first division of the image
    // 2. Receive the smoothed divisions from slave executors
    // 3. Write the smoothed image to the JPEG File
    if (task_id == MASTER)
    {
        // Smooth the first division of the image
        auto filteredImage =
            new unsigned char[input_jpeg.height * input_jpeg.width *
                              input_jpeg.num_channels];
#pragma unroll(32)
        for (int height = cuts[MASTER]; height < cuts[MASTER + 1]; ++height)
        {
#pragma unroll(32)
            for (int width = 1; width < input_jpeg.width - 1; ++width)
            {
                int sum_r = 0, sum_g = 0, sum_b = 0;
#pragma unroll(FILTER_SIZE)
                for (int i = -1; i <= 1; ++i)
                {
#pragma unroll(FILTER_SIZE)
                    for (int j = -1; j <= 1; ++j)
                    {
                        int channel_value_r =
                            input_jpeg.buffer[((height + i) * input_jpeg.width +
                                               (width + j)) *
                                              input_jpeg.num_channels];
                        int channel_value_g =
                            input_jpeg.buffer[((height + i) * input_jpeg.width +
                                               (width + j)) *
                                                  input_jpeg.num_channels +
                                              1];
                        int channel_value_b =
                            input_jpeg.buffer[((height + i) * input_jpeg.width +
                                               (width + j)) *
                                                  input_jpeg.num_channels +
                                              2];
                        sum_r += channel_value_r * filter[i + 1][j + 1];
                        sum_g += channel_value_g * filter[i + 1][j + 1];
                        sum_b += channel_value_b * filter[i + 1][j + 1];
                    }
                }
                filteredImage[(height * input_jpeg.width + width) *
                              input_jpeg.num_channels] =
                    static_cast<unsigned char>(std::round(sum_r));
                filteredImage[(height * input_jpeg.width + width) *
                                  input_jpeg.num_channels +
                              1] =
                    static_cast<unsigned char>(std::round(sum_g));
                filteredImage[(height * input_jpeg.width + width) *
                                  input_jpeg.num_channels +
                              2] =
                    static_cast<unsigned char>(std::round(sum_b));
            }
        }

        // Receive the smoothed divisions from each slave executors
        for (int i = MASTER + 1; i < num_tasks; ++i)
        {
            unsigned char* start_pos =
                filteredImage +
                cuts[i] * input_jpeg.width * input_jpeg.num_channels;
            int length = (cuts[i + 1] - cuts[i]) * input_jpeg.width *
                         input_jpeg.num_channels;
            MPI_Recv(start_pos, length, MPI_UNSIGNED_CHAR, i, TAG_GATHER,
                     MPI_COMM_WORLD, &status);
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                                  start_time);

        // Save the Smoothed Image
        const char* output_filepath = argv[2];
        std::cout << "Output file to: " << output_filepath << "\n";
        JPEGMeta output_jpeg{filteredImage, input_jpeg.width, input_jpeg.height,
                             input_jpeg.num_channels, input_jpeg.color_space};
        if (write_to_jpeg(output_jpeg, output_filepath))
        {
            std::cerr << "Failed to write output JPEG to file\n";
            MPI_Finalize();
            return -1;
        }

        // Release the memory
        delete[] input_jpeg.buffer;
        delete[] filteredImage;
        std::cout << "Smoothing Complete!" << std::endl;
        std::cout << "Execution Time: " << elapsed_time.count()
                  << " milliseconds\n";
    }
    // The tasks for the slave executor
    // 1. Smooth the image
    // 2. Send the smoothed division back to the master executor
    else
    {
        // Smooth the image
        int length = (cuts[task_id + 1] - cuts[task_id]) * input_jpeg.width *
                     input_jpeg.num_channels;
        auto result = new unsigned char[length];
        for (int height = cuts[task_id]; height < cuts[task_id + 1]; ++height)
        {
            for (int width = 1; width < input_jpeg.width - 1; ++width)
            {
                int sum_red = 0, sum_green = 0, sum_blue = 0;
                for (int i = -1; i <= 1; i++)
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        int channel_value_red =
                            input_jpeg.buffer[((height + i) * input_jpeg.width +
                                               (width + j)) *
                                              input_jpeg.num_channels];
                        int channel_value_green =
                            input_jpeg.buffer[((height + i) * input_jpeg.width +
                                               (width + j)) *
                                                  input_jpeg.num_channels +
                                              1];
                        int channel_value_blue =
                            input_jpeg.buffer[((height + i) * input_jpeg.width +
                                               (width + j)) *
                                                  input_jpeg.num_channels +
                                              2];
                        sum_red += channel_value_red * filter[i + 1][j + 1];
                        sum_green += channel_value_green * filter[i + 1][j + 1];
                        sum_blue += channel_value_blue * filter[i + 1][j + 1];
                    }
                }
                result[(height * input_jpeg.width + width) *
                           input_jpeg.num_channels -
                       cuts[task_id] * input_jpeg.width *
                           input_jpeg.num_channels] =
                    static_cast<unsigned char>(std::round(sum_red));
                result[(height * input_jpeg.width + width) *
                           input_jpeg.num_channels +
                       1 -
                       cuts[task_id] * input_jpeg.width *
                           input_jpeg.num_channels] =
                    static_cast<unsigned char>(std::round(sum_green));
                result[(height * input_jpeg.width + width) *
                           input_jpeg.num_channels +
                       2 -
                       cuts[task_id] * input_jpeg.width *
                           input_jpeg.num_channels] =
                    static_cast<unsigned char>(std::round(sum_blue));
            }
        }

        // Send the smoothed image back to the master
        MPI_Send(result, length, MPI_UNSIGNED_CHAR, MASTER, TAG_GATHER,
                 MPI_COMM_WORLD);

        // Release the memory
        delete[] result;
    }

    MPI_Finalize();
    return 0;
}
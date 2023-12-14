#include <algorithm>
#include "simple_ml_openacc.hpp"

void matrix_dot_openacc(const float* A, const float* B,
    float* C, size_t m, size_t n, size_t k)
{
    // BEGIN YOUR CODE
    memset(C, 0, m * k * sizeof(float));
#pragma acc kernels

    for (size_t i = 0; i < m; ++i)
    {
        auto A_row = A + i * n;
        auto C_row = C + i * k;
#pragma acc kernels

        for (size_t j = 0; j < n; ++j)
        {
            auto A_val = A_row[j];
            auto B_row = B + j * k;
#pragma acc kernels

            for (size_t l = 0; l < k; ++l)
            {
                C_row[l] += A_val * B_row[l];
            }
        }
    }

    // END YOUR CODE
}

void matrix_dot_trans_openacc(const float* A, const float* B, float* C, size_t n, size_t m, size_t k)
{
    // BEGIN YOUR CODE
    memset(C, 0, m * k * sizeof(float));
#pragma acc kernels

    for (size_t i = 0; i < n; ++i)
    {
        auto A_row = A + i * m;
        auto B_row = B + i * k;
#pragma acc kernels

        for (size_t j = 0; j < m; ++j)
        {
            auto A_val = A_row[j];
            auto C_row = C + j * k;
#pragma acc kernels

            for (size_t l = 0; l < k; ++l)
            {
                C_row[l] += A_val * B_row[l];
            }
        }
    }
// END YOUR CODE
}

void matrix_trans_dot_openacc(const float* A, const float* B, float* C, size_t m, size_t n, size_t k)
{
    // BEGIN YOUR CODE
    memset(C, 0, m * k * sizeof(float));
#pragma acc kernels

    for (size_t i = 0; i < n; ++i)
    {
        auto A_row = A + i * m;
        auto B_row = B + i;
#pragma acc kernels

        for (size_t j = 0; j < m; ++j)
        {
            auto A_val = A_row[j];
            auto C_row = C + j * k;
#pragma acc kernels

            for (size_t l = 0; l < k; ++l)
            {
                C_row[l] += A_val * B_row[l];
            }
        }
    }

    // END YOUR CODE
}

void matrix_minus_openacc(float* A, const float* B, size_t m, size_t n)
{
    // BEGIN YOUR CODE
#pragma acc kernels
    for (size_t i = 0; i < m * n; ++i)
    {
        A[i] -= B[i];
    }

    // END YOUR CODE
}

void matrix_mul_scalar_openacc(float* C, float scalar, size_t m, size_t n)
{
    // BEGIN YOUR CODE
#pragma acc kernels
    for (size_t i = 0; i < m * n; ++i)
    {
        C[i] *= scalar;
    }

    // END YOUR CODE
}

void matrix_div_scalar_openacc(float* C, float scalar, size_t m, size_t n)
{
    // BEGIN YOUR CODE
    float reciprocal_scalar = 1 / scalar;
#pragma acc kernels
    for (size_t i = 0; i < m * n; ++i)
    {
        C[i] *= reciprocal_scalar;
    }
    // END YOUR CODE
}

void matrix_softmax_normalize_openacc(float* C, size_t m, size_t n)
{
    // BEGIN YOUR CODE
#pragma acc kernels
    for (auto C_row = C; C_row < C + m * n; C_row += n)
    {
        // Find the maximum value in the row for numerical stability
        auto max_val = *std::max_element(C_row, C_row + n);

        auto sum = 0.0f;
#pragma acc kernels
        for (size_t i = 0; i < n; ++i)
        {
            C_row[i] = expf(C_row[i] - max_val);
            sum += C_row[i];
        }
        auto sum_inv = 1 / sum;
#pragma acc kernels
        for (size_t i = 0; i < n; ++i)
        {
            C_row[i] *= sum_inv;
        }
    }
    // END YOUR CODE
}

void vector_to_one_hot_matrix_openacc(const unsigned char* y, float* Y, size_t m, size_t n)
{
    // BEGIN YOUR CODE
    memset(Y, 0, m * n * sizeof(float));

#pragma acc kernels
    for (size_t i = 0; i < m; ++i)
    {
        auto Y_row = Y + i * n;
        auto y_val = y[i];
        Y_row[y_val] = 1;
    }

    // END YOUR CODE
}

void softmax_regression_epoch_openacc(const float* X, const unsigned char* y,
    float* theta, size_t m, size_t n, size_t k,
    float lr, size_t batch)
{
    // BEGIN YOUR CODE
    auto const h_batch = new float[batch * k];
    auto const Y_batch = new float[batch * k];
    auto const grad_batch = new float[n * k];

    for (size_t i = 0; i < m; i += batch)
    {
        auto X_batch = X + i * n;

        matrix_dot_openacc(X_batch, theta, h_batch, batch, n, k);
        matrix_softmax_normalize_openacc(h_batch, batch, k);
        vector_to_one_hot_matrix_openacc(y + i, Y_batch, batch, k);
        matrix_minus_openacc(h_batch, Y_batch, batch, k);
        matrix_dot_trans_openacc(X_batch, h_batch, grad_batch, batch, n, k);
        matrix_mul_scalar_openacc(grad_batch, lr / static_cast<float>(batch), n, k);
        matrix_minus_openacc(theta, grad_batch, n, k);
    }

    delete[] h_batch;
    delete[] Y_batch;
    delete[] grad_batch;
    // END YOUR CODE
}

void
train_softmax_openacc(const DataSet* train_data, const DataSet* test_data, size_t num_classes, size_t epochs, float lr,
    size_t batch)
{
    /*
    Example function to fully train a softmax regression classifier
    */
    size_t size = train_data->input_dim * num_classes;
    float* theta = new float[size];
    memset(theta, 0, size * sizeof(float));
    size_t size_tr = train_data->images_num * num_classes;
    size_t size_te = test_data->images_num * num_classes;
    float* train_result = new float[size_tr];
    float* test_result = new float[size_te];
    float train_loss, train_err, test_loss, test_err;
    std::cout << "| Epoch | Train Loss | Train Err | Test Loss | Test Err |" << std::endl;
    std::chrono::milliseconds elapsed_time;
    auto start_time = std::chrono::high_resolution_clock::now();
    for (size_t epoch = 0; epoch < epochs; epoch++)
    {
        // BEGIN YOUR CODE
        softmax_regression_epoch_openacc(train_data->images_matrix,
            train_data->labels_array,
            theta,
            train_data->images_num,
            train_data->input_dim,
            num_classes,
            lr,
            batch);

        matrix_dot_openacc(train_data->images_matrix,
            theta,
            train_result,
            train_data->images_num,
            train_data->input_dim,
            num_classes);
        matrix_dot_openacc(test_data->images_matrix,
            theta,
            test_result,
            test_data->images_num,
            test_data->input_dim,
            num_classes);

        // result = X @ theta
        // END YOUR CODE
        train_loss = mean_softmax_loss_openacc(train_result, train_data->labels_array, train_data->images_num,
            num_classes);
        test_loss = mean_softmax_loss_openacc(test_result, test_data->labels_array, test_data->images_num, num_classes);
        train_err = mean_err_openacc(train_result, train_data->labels_array, train_data->images_num, num_classes);
        test_err = mean_err_openacc(test_result, test_data->labels_array, test_data->images_num, num_classes);
        std::cout << "|  " << std::setw(4) << std::right << epoch << " |    "
                  << std::fixed << std::setprecision(5) << train_loss << " |   "
                  << std::fixed << std::setprecision(5) << train_err << " |   "
                  << std::fixed << std::setprecision(5) << test_loss << " |  "
                  << std::fixed << std::setprecision(5) << test_err << " |" << std::endl;
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    elapsed_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
            start_time);
    std::cout << "Execution Time: " << elapsed_time.count()
              << " milliseconds\n";

    // END YOUR CODE
    delete[] theta;
    delete[] train_result;
    delete[] test_result;
}

float mean_softmax_loss_openacc(const float* result, const unsigned char* labels_array, size_t images_num,
    size_t num_classes)
{
    // BEGIN YOUR CODE
    auto loss = 0.0f;

    for (size_t i = 0; i < images_num; ++i)
    {
        auto result_row = result + i * num_classes;
        auto true_class = labels_array[i];

        auto tmp = 0.0f;
        for (size_t j = 0; j < num_classes; ++j)
        {
            tmp += expf(result_row[j] - result_row[true_class]);
        }
        loss += logf(tmp);
    }

    loss /= static_cast<float>(images_num);
    return loss;
    // END YOUR CODE
}

float mean_err_openacc(const float* result, const unsigned char* labels_array, size_t images_num, size_t num_classes)
{
    // BEGIN YOUR CODE
    auto Y = new float[images_num * num_classes];
    vector_to_one_hot_matrix_openacc(labels_array, Y, images_num, num_classes);
    auto err = 0.0f;

    for (size_t i = 0; i < images_num; ++i)
    {
        auto Y_row = Y + i * num_classes;
        auto result_row = result + i * num_classes;
        auto max_val = result_row[0];
        auto max_idx = 0;
        for (size_t j = 1; j < num_classes; ++j)
        {
            if (result_row[j] > max_val)
            {
                max_val = result_row[j];
                max_idx = j;
            }
        }
        if (Y_row[max_idx] != 1)
        {
            ++err;
        }
    }
    err /= static_cast<float>(images_num);
    delete[] Y;
    return err;

    // END YOUR CODE
}

void matrix_mul_openacc(float* A, const float* B, size_t size)
{
    // BEGIN YOUR CODE

    // END YOUR CODE
}

void
nn_epoch_openacc(const float* X, const unsigned char* y, float* W1, float* W2, size_t m, size_t n, size_t l, size_t k,
    float lr, size_t batch)
{
    // BEGIN YOUR CODE

    // END YOUR CODE
}

void train_nn_openacc(const DataSet* train_data, const DataSet* test_data, size_t num_classes, size_t hidden_dim,
    size_t epochs, float lr, size_t batch)
{
    size_t size_w1 = train_data->input_dim * hidden_dim;
    size_t size_w2 = hidden_dim * num_classes;
    float* W1 = new float[size_w1];
    float* W2 = new float[size_w2];
    std::mt19937 rng;
    rng.seed(0);
    std::normal_distribution<float> dist(0.0, 1.0);
    for (size_t i = 0; i < size_w1; i++)
    {
        W1[i] = dist(rng);
    }
    for (size_t i = 0; i < size_w2; i++)
    {
        W2[i] = dist(rng);
    }
    matrix_div_scalar_openacc(W1, sqrtf(hidden_dim), train_data->input_dim, hidden_dim);
    matrix_div_scalar_openacc(W2, sqrtf(num_classes), hidden_dim, num_classes);
    size_t size_tr = train_data->images_num * num_classes;
    size_t size_te = test_data->images_num * num_classes;
    float* train_result = new float[size_tr];
    float* test_result = new float[size_te];
    float train_loss, train_err, test_loss, test_err;
    std::cout << "| Epoch | Train Loss | Train Err | Test Loss | Test Err |" << std::endl;
    std::chrono::milliseconds elapsed_time;
    // BEGIN YOUR CODE

    auto start_time = std::chrono::high_resolution_clock::now();
    for (size_t epoch = 0; epoch < epochs; epoch++)
    {
        train_loss = mean_softmax_loss_openacc(train_result, train_data->labels_array, train_data->images_num,
            num_classes);
        test_loss = mean_softmax_loss_openacc(test_result, test_data->labels_array, test_data->images_num, num_classes);
        train_err = mean_err_openacc(train_result, train_data->labels_array, train_data->images_num, num_classes);
        test_err = mean_err_openacc(test_result, test_data->labels_array, test_data->images_num, num_classes);
        std::cout << "|  " << std::setw(4) << std::right << epoch << " |    "
                  << std::fixed << std::setprecision(5) << train_loss << " |   "
                  << std::fixed << std::setprecision(5) << train_err << " |   "
                  << std::fixed << std::setprecision(5) << test_loss << " |  "
                  << std::fixed << std::setprecision(5) << test_err << " |" << std::endl;
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    elapsed_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
            start_time);
    std::cout << "Execution Time: " << elapsed_time.count()
              << " milliseconds\n";

    // END YOUR CODE
    delete[] W1;
    delete[] W2;
    delete[] train_result;
    delete[] test_result;
}

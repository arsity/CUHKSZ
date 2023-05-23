#include "../headers/Triangle.hpp"
#include "../headers/rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
// add some other header files you need

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(const Eigen::Vector3f &eye_pos) {
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1,
        -eye_pos[2], 0, 0, 0, 1;

    view = translate * view;
    // std::clog << "view" << std::endl << view << std::endl;  // check data

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle,
                                 const Eigen::Vector3f &T = {0, 0, 0},
                                 const Eigen::Vector3f &S = {1, 1, 1},
                                 const Eigen::Vector3f &P0 = {0, 0, 0},
                                 const Eigen::Vector3f &P1 = {0, 0, -1}) {

    //Step 1: Build the Translation Matrix M_trans:

    //Step 2: Build the Scale Matrix S_trans:

    //Step 3: Implement Rodrigues' Rotation Formular, rotation by angle theta around axix u, then get the model matrix
    // The axis u is determined by two points, u = P1-P0: Eigen::Vector3f P0 ,Eigen::Vector3f P1
    // Create the model matrix for rotating the triangle around a given axis. // Hint: normalize axis first

    //Step 4: Use Eigen's "AngleAxisf" to verify your Rotation
    //Eigen::AngleAxisf rotation_vector(radian, Vector3f(axis[0], axis[1], axis[2]));
    //Eigen::Matrix3f rotation_matrix;
    //rotation_m = rotation_vector.toRotationMatrix();

    // Step 1 Implement
    Eigen::Matrix4f M_trans = Eigen::Matrix4f::Identity();
    for (int i = 0; i < 3; ++i) {
        M_trans(i, 3) = T(i);
    }

    // Step 2 Implement
    Eigen::Matrix4f S_trans = Eigen::Matrix4f::Identity();
    for (int i = 0; i < 3; ++i) {
        S_trans(i, i) = S(i);
    }

    // Step 3 Implement
    Eigen::Vector3f u{P1 - P0};
    u.normalize();
    Eigen::Matrix3f lastMatrix = Eigen::Matrix3f::Zero();
    lastMatrix(0, 1) = -u(2);
    lastMatrix(1, 0) = u(2);
    lastMatrix(0, 2) = u(1);
    lastMatrix(2, 0) = -u(1);
    lastMatrix(1, 2) = -u(0);
    lastMatrix(2, 1) = u(0);

    Eigen::Matrix3f
        modelMatrix = cos(rotation_angle * MY_PI / 180) * Eigen::Matrix3f::Identity()
        + (1 - cos(rotation_angle * MY_PI / 180)) * u * u.transpose()
        + sin(rotation_angle * MY_PI / 180) * lastMatrix;

    Eigen::Matrix4f model_Matrix = Eigen::Matrix4f::Identity();
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            model_Matrix(i, j) = modelMatrix(i, j);
        }
    }

    // Step 4 Implement
    Eigen::AngleAxisf rotation_vector(rotation_angle * MY_PI / 180, u);
    Eigen::Matrix3f rotation_matrix = rotation_vector.toRotationMatrix();

    // check, no assert due to precision loss
    std::cout << "Eigen Matrix:" << std::endl << rotation_matrix << std::endl;
    std::cout << "My Matrix:" << std::endl << modelMatrix << std::endl;

    return S_trans * model_Matrix * M_trans;
}

// helper function for projection matrix
Eigen::Matrix4f get_orthogonal_matrix(float l, float r, float b, float t, float n, float f) {
    Eigen::Matrix4f res = Eigen::Matrix4f::Identity();
    res(0, 0) = float(2) / (r - l);
    res(1, 1) = float(2) / (t - b);
    res(2, 2) = float(2) / (n - f);
    res(0, 3) = -(r + l) / (r - l);
    res(1, 3) = -(t + b) / (t - b);
    res(2, 3) = -(n + f) / (n - f);
    return res;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar) {
    // Implement this function


    // Create the projection matrix for the given parameters.
    // Then return it.

    // frustum -> cubic

    // orthographic projection

    // squash all transformations

    // std::clog << "projection" << std::endl << projection << std::endl; //check

    Eigen::Matrix4f M_persp = Eigen::Matrix4f::Zero();
    M_persp(0, 0) = zNear;
    M_persp(1, 1) = zNear;
    M_persp(3, 2) = 1;
    M_persp(2, 2) = zNear + zFar;
    M_persp(2, 3) = -zNear * zFar;

    auto angle = eye_fov / 2 / 180 * MY_PI;
    auto h = abs(zNear) * tan(angle);
    auto w = h * aspect_ratio;
    auto M_ortho = get_orthogonal_matrix(-w, w, -h, h, zNear, zFar);

    Eigen::Matrix4f projection = M_ortho * M_persp;

    // debug line
    std::clog << "projection" << std::endl << projection << std::endl;

    return projection;
}

int main(int argc, const char** argv) {
    float angle = 0;
    bool command_line = false;
    std::string filename = "result.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        } else {
            return 0;
        }
    }

    rst::rasterizer r(1024, 1024);
    // define your eye position "eye_pos" to a proper position
    const Eigen::Vector3f eye_pos{0, 0, 10};

    // define a triangle named by "pos" and "ind"

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    // added parameters for get_projection_matrix(float eye_fov, float aspect_ratio,float zNear, float zFar)
    constexpr float eye_fov = 60; // use degree notation
    constexpr float aspect_ratio = 1; // use cubic
    constexpr float zNear = -20;
    constexpr float zFar = -80;

    Eigen::Vector3f T = Eigen::Vector3f::Zero();
    Eigen::Vector3f S = Eigen::Vector3f::Ones();
    Eigen::Vector3f P0{0, 0, 0};
    Eigen::Vector3f P1{1, 1, 1};

    // Eigen::Vector3f axis(0, 0, 1);
    Eigen::Vector3f axis(1, 0, 0);

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);
        r.set_model(get_model_matrix(angle, T, S, P0, P1));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(eye_fov, aspect_ratio, zNear, zFar));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(1024, 1024, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);
        r.set_model(get_model_matrix(angle, T, S, P0, P1));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(eye_fov, aspect_ratio, zNear, zFar));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(1024, 1024, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);

        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';
        std::clog << "angle: " << angle << std::endl;

        if (key == 'a') {
            angle += 10;
        } else if (key == 'd') {
            angle -= 10;
        }
    }

    return 0;
}

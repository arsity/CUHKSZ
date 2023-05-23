cmake_minimum_required(VERSION 3.24)
project(CSC4140_Proj1)

set(CMAKE_CXX_STANDARD 20)

# to fix "unsupported/Eigen/CXX11/Tensor: No such file or directory"
add_definitions(-DOPENCV_DISABLE_EIGEN_TENSOR_SUPPORT)

# to avoid missing headers
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories("/usr/include/eigen3/")

add_executable(CSC4140_Proj1 main.cpp)

# to avoid missing headers
target_link_libraries(CSC4140_Proj1 ${OpenCV_LIBS})

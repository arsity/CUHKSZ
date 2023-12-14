cmake_minimum_required(VERSION 3.10)
project(project4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(src)

add_executable(simple_ml_ext
        src/simple_ml_ext.hpp
        src/simple_ml_ext.cpp
        src/softmax_classifier.cpp
)

add_executable(simple_ml_openacc
        src/simple_ml_openacc.hpp
        src/simple_ml_openacc.cpp
        src/simple_ml_ext.hpp
        src/simple_ml_ext.cpp
        src/softmax_classifier_openacc.cpp
)

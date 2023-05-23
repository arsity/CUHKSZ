cmake_minimum_required(VERSION 3.24)
project(CSC4140_Assignment2)

set(CMAKE_CXX_STANDARD 20)

# print compile warnings
if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # additional warnings
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# enable address sanitizer for debug
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    # using regular Clang or AppleClang
    # build type is Debug
    add_compile_options(-g -O2)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

# to avoid missing headers
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories("/usr/include/eigen3/")

add_executable(CSC4140_Assignment2 src/main.cpp src/rasterizer.cpp src/Triangle.cpp)

# to avoid missing headers
target_link_libraries(CSC4140_Assignment2 ${OpenCV_LIBS})

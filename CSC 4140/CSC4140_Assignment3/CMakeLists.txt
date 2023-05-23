cmake_minimum_required(VERSION 3.8)
project(Assignment3)

#-------------------------------------------------------------------------------
# Build options
#-------------------------------------------------------------------------------
option(BUILD_DEBUG     "Build with debug settings"    OFF)
option(BUILD_DOCS      "Build documentation"          OFF)
option(BUILD_CUSTOM    "Build without reference"      OFF)

set(BUILD_DEBUG ${BUILD_DEBUG} CACHE BOOL "Build debug" FORCE)

if (BUILD_DEBUG)
  set(CMAKE_BUILD_TYPE Debug)
endif()

## Add debug options
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

#-------------------------------------------------------------------------------
# Set target
#-------------------------------------------------------------------------------
set(APPLICATION_SOURCE
    
    src/texture.cpp
    src/triangulation.cpp
    src/svgparser.cpp
    src/transforms.cpp
    src/rasterizer.cpp
    src/drawrend.cpp
    src/svg.cpp
    src/main.cpp
    # Add headers for the sake of Xcode/Visual Studio projects
    src/rasterizer.h
    src/drawrend.h
    src/svg.h
    src/svgparser.h
    src/texture.h
    src/transforms.h
    src/triangulation.h
)

if (WIN32)
    include_directories(src/dirent)
    list(APPEND APPLICATION_SOURCE src/dirent/dirent.c)
endif(WIN32)

add_executable(draw ${APPLICATION_SOURCE})

target_include_directories(draw PUBLIC src)

set(CGL_INCLUDE_DIRS CGL/include CGL/deps/glew/include CGL/deps/glfw/include)

#-------------------------------------------------------------------------------
# Find dependencies
#-------------------------------------------------------------------------------
add_subdirectory(CGL)
target_include_directories(draw PUBLIC ${CGL_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CGL_CXX_FLAGS}")

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
target_link_libraries(draw PUBLIC OpenGL::GL)
target_link_libraries(draw PUBLIC OpenGL::GLU)

target_link_libraries(draw PRIVATE CGL)

#-------------------------------------------------------------------------------
# Add subdirectories
#-------------------------------------------------------------------------------

# build documentation
if(BUILD_DOCS)
  find_package(DOXYGEN)
  if(DOXYGEN_FOUND AND BUILD_DOCS)
    add_subdirectory(docs)
  endif()
endif()

# Install settings
set(CMAKE_INSTALL_PREFIX "${Assignment3_SOURCE_DIR}/")

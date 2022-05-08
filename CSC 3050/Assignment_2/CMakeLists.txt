cmake_minimum_required(VERSION 3.18)
project(Assignment_2)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

add_executable(Assignment_2
        src/Dump.cpp
        src/Dump.h
        src/JCommand.cpp
        src/JCommand.h
        src/RCommand.cpp
        src/RCommand.h
        src/tester.cpp
        src/tester.h
        src/ICommand.cpp
        src/ICommand.h
        src/DataSegment.cpp
        src/DataSegment.h
        src/TextSegment.cpp
        src/TextSegment.h
        src/Execution.cpp
        src/Execution.h)

cmake_minimum_required(VERSION 3.18.4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Assignment_1 VERSION 1.0)

include_directories(src)
include_directories(headers)

add_executable(Assignment_1
        src/tester.cpp
        src/LabelTable.cpp headers/LabelTable.h
        src/Phase1.cpp headers/Phase1.h
        src/Phase2.cpp headers/Phase2.h
        src/MipsVar.cpp headers/MipsVar.h
        src/MipsRCom.cpp headers/MipsRCom.h
        src/MipsJCom.cpp headers/MipsJCom.h
        src/MipsIcom.cpp headers/MipsIcom.h)
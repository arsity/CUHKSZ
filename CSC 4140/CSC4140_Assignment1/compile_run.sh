#!/bin/bash

path=cmake-build-release

mkdir $path || exit
cmake -DCMAKE_BUILD_TYPE=Release -B$path
cmake --build $path
./$path/CSC4140_Proj1
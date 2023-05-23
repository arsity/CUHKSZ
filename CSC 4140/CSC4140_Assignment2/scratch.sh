#!/bin/bash

mkdir "build" || true

cmake -DCMAKE_BUILD_TYPE=Release -B "build"
cmake --build "build"
./build/CSC4140_Assignment2 "$@"

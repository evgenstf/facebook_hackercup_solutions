#!/bin/bash
if [ "$#" -ne 1 ]; then
    echo "Usage: ./build.sh <problem>"
    exit
fi

mkdir problems/$1/build 2> /dev/null
cat base/async_solution.h problems/$1/solution/solution.h base/launcher.cpp \
> problems/$1/build/assembled_solution.cpp &&\
g++ problems/$1/build/assembled_solution.cpp -o problems/$1/build/launch -std=c++17 -pthread

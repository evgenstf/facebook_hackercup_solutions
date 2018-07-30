#!/bin/bash -e
if [ "$#" -ne 2 ]; then
    echo "Usage: ./run_solution.sh <problem> <test>"
    exit
fi

echo "build solution"
./build.sh $1
echo "start solution"
time problems/$1/build/launch < problems/$1/tests/$2.txt > problems/$1/answers/$2.ans

echo "----------------------------"
head problems/$1/answers/$2.ans

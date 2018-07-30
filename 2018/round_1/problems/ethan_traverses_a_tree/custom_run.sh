#!/bin/bash
if [ "$#" -ne 1 ]; then
    echo "Usage: ./run_solution.sh <test>"
    exit
fi

cd ../../
./run_solution.sh ethan_traverses_a_tree $1

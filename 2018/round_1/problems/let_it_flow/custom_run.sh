#!/bin/bash
if [ "$#" -ne 1 ]; then
    echo "Usage: ./run_solution.sh <test>"
    exit
fi

cd ../../
./run_solution.sh let_it_flow $1

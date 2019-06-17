#!/bin/bash
if [ "$#" -ne 1 ]; then
    echo "Usage: ./run_solution.sh <test>"
    exit
fi

cd ../../
./run_solution.sh leapfrog_ch_1 $1

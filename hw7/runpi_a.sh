#!/usr/bin/bash

# script to run piprog_A for 1, 2, 4 number of threads

export OMP_NUM_THREADS
for x in 1 2 4
do
    OMP_NUM_THREADS=$x
    ./piprog_A
done


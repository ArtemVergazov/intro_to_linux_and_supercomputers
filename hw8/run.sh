#!/usr/bin/bash
# Syntax of run: ./run.sh -n #P -i #I 
# #P is number of threads
# #I is number of trials

while getopts n:i: flag
do
    case "${flag}" in
	n) threads=${OPTARG};;
	i) iterations=${OPTARG};;
    esac
done

module load mpi/openmpi-3.1.2
mpicc -o pim -O3 prog.c -lm
mpirun -np $threads -mca btl_base_warn_component_unused 0 ./pim $iterations

# Output:
# #P = 1, #I = 1638400000
# Trials: 1638400000 Ncirc: 1286809573 Threads: 1 Elapsed: 74.69 PI: 3.14162493 dpi: 0.0001

# #P = 4, #I = 1638400000 trials:
# Trials: 6553600000 Ncirc: 5147203671 Threads: 4 Elapsed: 43.03 PI: 3.14160380 dpi: 5e-05

# Speedup = 74.69 / 43.03 = 1.74


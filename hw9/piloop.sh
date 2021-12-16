#!/usr/bin/bash

module load compilers/gcc-7.3.1

for ((i=1; i< 8; i++))
do
	srun --ntasks=1 runPIomp.sh 10 &
done

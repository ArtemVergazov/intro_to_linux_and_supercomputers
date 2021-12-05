#!/usr/bin/bash
#SBATCH --output=piprog_G_2_%j.out
#SBATCH --partition=cpu
#SBATCH --nodes=1
#SBATCH --cpus-per-task=4
#SBATCH --time=10:00
#SBATCH --mem-per-cpu=10M

export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK:-4}

./piprog_G -s 1235791 -t 26214400000


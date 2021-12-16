#!/usr/bin/bash
#SBATCH --job-name=piomp
#SBATCH --output=piomp_%j.out
#SBATCH --partition=cpu        # -p cpu
#SBATCH --nodes=1              # -N 1 number of nodes
#SBATCH --cpus-per-task=8      # -c X cpus assigned to each task
#SBATCH --time=2:00
#SBATCH --mem=1G               # memory per node

export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK:-1}
echo "N-list: $SLURM_JOB_NODELIST #nodes: $SLURM_JOB_NUM_NODES"
echo "#tasks: $SLURM_NTASKS cpu: $SLURM_CPUS_PER_TASK"

module load compilers/gcc-7.3.1
make piomp &>/dev/null

mtrials=${1:-1}
numtrials=$mtrials
seed=$RANDOM

#echo " /usr/bin/time -p ./piomp -t $numtrials"
/usr/bin/time -p ./piomp -t $numtrials -a -t $numtrials -s $seed

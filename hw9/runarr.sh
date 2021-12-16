#!/usr/bin/bash
#SBATCH --job-name=piarr
#SBATCH --output=piarrJN.out
#SBATCH --partition=htc        # -p cpu
#SBATCH --cpus-per-task=1      # -c X cpus assigned to each task
#SBATCH --time=3:00
#SBATCH --mem=1G               # memory per node
#SBATCH --array=1-1000%100

export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK:-1}
echo "N-list: $SLURM_JOB_NODELIST #nodes: $SLURM_JOB_NUM_NODES"
echo "#tasks: $SLURM_NTASKS cpu: $SLURM_CPUS_PER_TASK array: $SLURM_ARRAY_TASK_ID"

#env | grep SLURM_ARRAY
#echo

module load compilers/gcc-7.3.1
make piomp &>/dev/null

mtrials=${1:-1}
numtrials=$mtrials
seed=$(($RANDOM*$SLURM_ARRAY_TASK_ID))

#echo " /usr/bin/time -p ./piomp -t $numtrials"
./piomp -t $numtrials -a -t $numtrials -s $seed | tee -a log
#echo END

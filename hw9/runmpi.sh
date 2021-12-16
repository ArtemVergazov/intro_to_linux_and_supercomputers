#!/usr/bin/bash
#SBATCH --job-name=pimpi
#SBATCH --output=pimpi_%j.out
#SBATCH --partition=cpu        # -p cpu
##SBATCH --nodes=4              # -N 4 number of nodes
#SBATCH --ntasks=4             # -n 4 total number of mpi processes
#SBATCH --cpus-per-task=4      # -c X cpus assigned to each task
#SBATCH --mincpus=5            # to ensure sufficient resources for MPI master thread
#SBATCH --time=2:00
#SBATCH --mem=1G               # memory per node

export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK:-1}
export MPI_NUM_PROCESS=${SLURM_JOB_NUM_NODES:-4}
echo "N-list: $SLURM_JOB_NODELIST #nodes: $SLURM_JOB_NUM_NODES"
echo "#tasks: $SLURM_NTASKS cpu: $SLURM_CPUS_PER_TASK"

export OMPI_MCA_btl_base_warn_component_unused=0
export OMPI_MCA_btl="self,tcp"
export OMPI_MCA_orte_base_help_aggregate=0

module load mpi/openmpi-3.1.2
module load compilers/gcc-7.3.1
make pimpi &>/dev/null

mtrials=${1:-1}
numtrials=$mtrials
seed=$RANDOM

/usr/bin/time -p mpirun ./pimpi -a -t $numtrials -s $seed

#!/usr/bin/bash

mtrials=${1:-1}

export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK:-1}
export MPI_NUM_PROCESS=${SLURM_NTASKS:-4}

export OMPI_MCA_btl_base_warn_component_unused=0
export OMPI_MCA_btl="self,tcp"
export OMPI_MCA_orte_base_help_aggregate=0

###export OMP_NUM_THREADS=2

module load mpi/openmpi-3.1.2
module load compilers/gcc-7.3.1
make pimpi &>/dev/null

numtrials=$mtrials

echo "nodelist: $SLURM_NODELIST Nodes: $SLURM_NNODES Tasks: $SLURM_NTASKS CPUs: $SLURM_NPROCS MPI: $MPI_NUM_PROCESS OMP: $OMP_NUM_THREADS"


#echo " /usr/bin/time -p mpirun -np $MPI_NUM_PROCESS pimpi -t $numtrials"
/usr/bin/time -p mpirun -np $MPI_NUM_PROCESS ./pimpi -s $RANDOM -t $numtrials -a | tee -a logMPI

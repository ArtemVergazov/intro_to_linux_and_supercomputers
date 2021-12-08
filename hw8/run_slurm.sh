#!/usr/bin/bash
#SBATCH --output=pim_%j.out
#SBATCH --partition=cpu
#SBATCH --nodes=1
#SBATCH --mem-per-cpu=10M

# Synatax of run: sbatch run_slurm.sh

ntrials=100000

tt=0

until (( $tt ))
do
    ntrials=$((ntrials*2)); echo $ntrials

    out=$(./run.sh -n 1 -i $ntrials | grep Elapsed )
    echo $out
    rt=$(echo $out | cut -d " " -f 8)
    rtint=$(echo "scale=0; $rt/1" | bc )
    echo $rtint
    if [ $rtint -gt 60 ] ; then
    	tt=1
    fi
done


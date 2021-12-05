#!/usr/bin/bash
#SBATCH --output=piprog_e_%j.out
#SBATCH --cpus-per-task=1
#SBATCH --partition=cpu
#SBATCH --nodes=1
#SBATCH --mem-per-cpu=10M

export OMP_NUM_THREADS=1

ntrials=1000000
genseed=1235791
tt=0

until (( $tt ))
do
    ntrials=$((ntrials*2)); echo $ntrials

    out=$( ./piprog_e -s $genseed -t $ntrials | grep Elapsed )
    echo $out
    rt=$( echo $out | cut -d " " -f 8 )
    rtint=$( echo "scale=0; $rt/1" | bc )
    if [ $rtint -gt 300 ] ; then
	tt=1
    fi
done


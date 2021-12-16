#!/usr/bin/bash

time="2:00"
mem=1G
queue=cpu

for tasks in 2 4 8
do
    nodes=$tasks
    for ((cpus=1; cpus < 9; cpus*=2))
    do
        mincpus=$((cpus+1))
        echo "submit $queue output=PImpi_${tasks}_${cpus}.out -N $nodes -n $tasks -c $cpus --mincpus=$mincpus --mem=$mem --time=$time"

        sbatch -J PI-${tasks}-${cpus} -p $queue --output=PImpi_${tasks}_${cpus}.out -N $nodes -n $tasks -c $cpus \
                                                --mincpus=$mincpus --mem=$mem --time=$time runPImpi.sh 10
        sleep 5
    done
done
echo END


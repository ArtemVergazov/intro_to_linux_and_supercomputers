#!/usr/bin/bash

export NUM_THREADS
NTRIAL=5

function pthr () {
	rt=$(/usr/bin/time -p ./pthr_atomic 2>&1 | grep real | cut -d" " -f 2)
	echo $rt
}

for x in 1 2 3 4
do
	NUM_THREADS=$x
	mintime=1000000
	echo ""

	for (( k=0; k < $NTRIAL; k++ ))
	do
		echo -n "trial $k:"	# the -n switch will not put \n

		rt=`pthr`
		echo -n " $rt	"
		ss=$(echo "$rt < $mintime" | bc)	# use bc for float comparisons
		if [[ $ss -eq 1 ]]
		then
			mintime=$rt
			#echo "$ss assign $mintime from $rt"
		fi
	done
	echo " "
	echo "NUM_THREADS: $NUM_THREADS min_real_time: $mintime"
done

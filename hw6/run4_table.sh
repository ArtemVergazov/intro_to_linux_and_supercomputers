#!/usr/bin/bash

# bash script to make a table of runtime comparison between mutex, semaphore and atomic usage
# made by Vsevolod Avilkin
export NUM_THREADS
export N_ITER=100000000
NTRIAL=5

function pthr_mutex () {
	rt=$(/usr/bin/time -p ./pthr_mutex 2>&1 | grep real | cut -d" " -f 2)
	echo $rt
}
function pthr_semaphore () {
	rt=$(/usr/bin/time -p ./pthr_semaphore 2>&1 | grep real | cut -d" " -f 2)
	echo $rt
}
function pthr_atomic () {
	rt=$(/usr/bin/time -p ./pthr_atomic 2>&1 | grep real | cut -d" " -f 2)
	echo $rt
}
resolution[1]=mutex
resolution[2]=semaphore
resolution[3]=atomic
printf '%-20s: %.1e\n' "number of iterations" $N_ITER
printf '%-20s: %d\n' "number of trials"		 $NTRIAL
printf '%-15s Real time[s] for threads \n' "RACE"
printf '%-17s%-10d%-10d%-10d\n' "resolution" "1" "2" "4"

for y in 1 2 3
do
	printf '%-17s' "${resolution[$y]}"
	for x in 1 2 4
	do
		NUM_THREADS=$x
		mintime=1000000
		
		for (( k=0; k < $NTRIAL; k++ ))
		do
			rt=`pthr_${resolution[$y]}`
			ss=$(echo "$rt < $mintime" | bc)	# use bc for float comparisons
			if [[ $ss -eq 1 ]]
			then
				mintime=$rt
				#echo "$ss assign $mintime from $rt"
			fi
		done
		printf '%-10f' "$mintime"
	done
	echo ""
done

export -n N_ITER
export -n NUM_THREADS

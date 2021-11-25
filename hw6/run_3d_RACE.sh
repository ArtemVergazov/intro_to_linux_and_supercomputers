#!/usr/bin/bash
#Lab 6 Ex 3d
#bash script for the file pthr_d to deduce at what values of N_ITER the answer is correct
export N_ITER
NTRIAL=20

function pthr () {
	rt=$(./pthr_d 2>&1 | cut -d" " -f 3)
	echo $rt
}

for x in 500000000 50000000 5000000 500000 50000 5000 500 50 5
do
	N_ITER=$x
	correct=1
	echo ""

	for (( k=0; k < $NTRIAL; k++ ))
	do
		echo -n "trial $k:"	# the -n switch will not put \n

		rt=`pthr`
		echo " sum = $rt	"
		ss=$(echo "$rt == 0" | bc)	# use bc for float comparisons
		if [[ $ss -eq 0 ]]
		then
			correct=0
		fi
	done
	echo " "
	echo "N_ITER: $N_ITER correct: $correct"
done

#!/usr/bin/bash
dir=${1:-/usr/bin}

function func () {
    sizes=$(/bin/ls -l $1 | tr -s " " | cut -d " " -f 5)
    local large=0 #strict local variable
    for x in $sizes
    do
        if [ $x -gt $large ]
        then
        large=$x
        fi
    done
    echo $large
}

large=$(func $dir)
echo "largest size in directory $dir $large"


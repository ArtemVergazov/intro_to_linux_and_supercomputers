#!/usr/bin/bash

# Created by Artem.Vergazov@skoltech.ru on 11.23.21 10:41PM.
# Revision 1.0
#
# A toy bash program.
#

var="Hello, World!" # no spaces around the = sigh
prog=$0	# name of the script
echo prog: $prog

arg1=$1	# argument in lst position
echo "arg1: $arg1"

argd=${1:-10} # positional defults to 10 if not there
echo "arg2: $argd" # substitution
echo 'arg2: $argd' # does not substitute

newvar1=$var.new # create new names with existing value

newvar2=${var}XX # create new names using {}

newvar3=$(ls -l) # run cmd in a sub shell and return result
echo $newvar3

echo
echo
newvar4=`date `	# run cmd in same shell and return result
echo $newvar4

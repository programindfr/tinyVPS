#!/bin/bash

if [ $EUID -ne 0 ]	# exit 1 if not root
then
	exit 1
fi

_tinyvps="/home/<user>/tinyvps"

for machine in $(ls "$_tinyvps")
do
	if [ -d "$_tinyvps/$machine" ]
	then
		cd "$_tinyvps/$machine"
		./run.sh &
	fi
done

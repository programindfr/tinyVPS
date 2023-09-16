#!/bin/bash

_tinyvps="$HOME/tinyvps"

for machine in $(ls "$_tinyvps")
do
	if [ -d "$_tinyvps/$machine" ]
	then
		cd "$_tinyvps/$machine"
		"./run.sh" &
	fi
done

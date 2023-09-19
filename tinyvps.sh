#!/bin/bash

if [ $EUID -ne 0 ]	# exit 1 if not root
then
	exit 1
fi

_tinyvps="/home/<user>/.tinyvps"
_qemu=$(which qemu-system-x86_64)

if [ -z $_qemu ]	# exit 2 if qemu is not found
then
	exit 2
fi

for machine in $(ls "$_tinyvps")
do
	if [ -d "$_tinyvps/$machine" ]
	then
		$_qemu $(cat "$_tinyvps/$machine/config") 2>> "$_tinyvps/$machine/log" &
	fi
done

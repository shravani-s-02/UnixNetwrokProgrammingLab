#!/bin/bash
dir="$1"
if [ ! -z $dir ]
then
	if [ -d $dir ]
	then 
		echo "File Size"
		ls -l $dir | tail -n+2  | awk '{print $9, $5}'
	else 
		echo "$dir is an invalid directory"
	fi
else
	echo "No command line argument is given"
fi

#!/bin/bash
input_pid="$1"
if [ ! -z $input_pid ]
then
	lines=$(ps -eo pid | awk '$1 =='$input_pid'' || wc -l)
	if [ ! -z $lines ] 
	then
		ps -eo etime,pid,comm,pcpu,pmem | head -1
		while true
		do
			ps -eo etime,pid,comm,pcpu,pmem | awk '{if($2 =='$input_pid')print $0}'
			sleep 1
		done
	else
		echo "PID entered is not a currently runing process"
	fi
else
	echo "No command line argument is given"
fi

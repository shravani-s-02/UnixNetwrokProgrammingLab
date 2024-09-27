#!/bin/bash
file="$1"
if [ ! -z $file ]
then
	declare -A word_count
	while read -r line
	do
		for word in $line 
		do
			if [ -v word_count[$word] ]
			then
				((word_count[$word]++))
			else 
				word_count[$word]=1
			fi
		done 
	done < $file
	
	for word in "${!word_count[@]}" 
	do 
		echo $word "," ${word_count[$word]}
	done
else
	echo "No command line argument is given"
fi

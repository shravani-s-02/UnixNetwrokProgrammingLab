#!/bin/bash
input_file="$1"
output_file="$2"
if [ ! -z $input_file ] && [ ! -z $output_file ]
then
	if [ -r $input_file ] && [ -w $output_file ]
	then 
		while read -r line
		do
			for word in $line
			do
				echo $word | grep -x "[AEIOUaeiou].*" >> $output_file
			done
		done < $input_file
	else 
		echo "One or more of the files cannot be read"
	fi
else
	echo "Two command line arguments are required"
fi

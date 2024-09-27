#!/bin/bash
pass="$1"
if [ ! -z $pass ]
then
	upper_count=$(echo $pass | grep -o [A-Z] | tr -d "\n"] | wc -m)
	lower_count=$(echo $pass | grep -o [a-z] | tr -d "\n"] | wc -m)
	if [ ${#pass} > 9 ] && [[ $pass =~ [[:alpha:]] && $pass =~ [[:digit:]] ]] && [ $upper_count -ge 2 ] && [ $lower_count -ge 2 ] && [[ $pass =~ ^[A-Z](.*)$ ]] 
	then 
		echo "Password is strong"
	else
		echo "Password is weak"
	fi
	
else
	echo "No command line argument is given"
fi

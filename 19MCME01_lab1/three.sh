#!/bin/bash
cal_GCD(){
	if [ $2 -ne 0 ]
	then 
		temp=`cal_GCD $2 $(($1%$2))`
		echo $temp
	else
		echo $1
	fi
}

num1=$1
num2=$2

if [ ! -z $num1 ] && [ ! -z $num2 ]
then
	GCD=`cal_GCD $num1 $num2` 
	echo "GCD(Greatest Common Divisor) of $num1 and $num2 is $GCD"
else
	echo "Not enough command line arguments, there must be two numbers given"
fi

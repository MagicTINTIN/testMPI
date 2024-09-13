#!/usr/bin/bash

let ligne=0
ip=10\.1\.15\. #10.1.15

for ((i=$1; i<=$2; i++))
do
	ping -W 0,01 -c 1 $ip$i > /dev/null 2>&1
	let res=$?
	if (($res == 0))
       	then
		if test -n $3 & (($ligne==1))
		then 
			echo
		fi
		ligne=0
		echo $ip$i
	else 
		if test -n "$3"
		then
			ligne=1
			echo -n -
		fi
	
	fi
done

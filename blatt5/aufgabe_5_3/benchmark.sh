#!/bin/bash

# Run with increasing number of threads until we reach the max number of threads.
for num_threads in 1 2 4 8 16; do
	for iteration in 1 2 3 4 5; do
		echo -ne "$2,$num_threads,$iteration,"
		/usr/bin/time --format="%U,%e" ./$1 $2 $num_threads
		#echo "/usr/bin/time --format="%U,%e" ./$1 $2 $num_threads"
	done 
done

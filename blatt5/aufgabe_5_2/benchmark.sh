#!/bin/bash

# 1. Clear output file.
#printf "" > $4

echo "Benchmarking montecarlo_pi!"
echo "Approximating PI using $2 samples."

# 2. Run with increasing number of threads until we reach the max number of threads.
for num_threads in $(eval echo {1..$3});
do
    printf "\rBenchmarking... $num_threads/$3"
    echo "###" >> $4
    echo -e "$2\t$num_threads" >> $4
    (time $1 $2 $num_threads) >> $4 2>&1
done
echo ""
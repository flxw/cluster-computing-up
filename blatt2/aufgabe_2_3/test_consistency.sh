#!/bin/bash

# 1. Clear output file.
#printf "" > $4

echo "Testing consistency of montecarlo_pi!"
echo "Approximating PI using $2 samples."

# 2. Run several times to generate enough samples to test for consistency.
for i in $(eval echo {1..$4});
do
    printf "\rTesting... $i/$4"
    echo "###" >> $5
    echo -e "$2\t$3" >> $5
    (time $1 $2 $3) >> $5 2>&1
done
echo ""
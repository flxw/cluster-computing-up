#!/bin/bash

# extra node hostnames from stupid PBS_NODEFILE
IFS=$'\r\n' GLOBIGNORE='*' command eval  'NODEARRAY=($(sort -u $PBS_NODEFILE))'
COMPUTENODE="${NODEARRAY[0]}"

# copy executables over
for EXE in "montecarlo_pi" "affine_montecarlo_pi" "benchmark.sh" "test_consistency"; do
	scp "$EXE" "$COMPUTENODE:"
done

# run script on node
ssh "$COMPUTENODE" 'bash -s' << 'ENDSSH'
for EXE in "montecarlo_pi" "affine_montecarlo_pi"; do
	./benchmark.sh "./$EXE" 1000000000 16 benchmark_results_$EXE.txt

	for $SAMPLESIZE in 10000 100000 1000000 10000000 100000000 1000000000; do
		#./test_consistency "./$EXE" "$SAMPLESIZE" 8 100 consistency_results_$EXE.txt
	done
done
ENDSSH

# copy results back from computenode
for EXE in "montecarlo_pi" "affine_montecarlo_pi"; do
	scp "$COMPUTENODE:benchmark_results_$EXE.txt"   .
	scp "$COMPUTENODE:consistency_results_$EXE.txt" .
done

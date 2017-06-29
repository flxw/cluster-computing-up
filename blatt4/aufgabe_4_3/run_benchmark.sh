#!/bin/bash

# extra node hostnames from stupid PBS_NODEFILE
IFS=$'\r\n' GLOBIGNORE='*' command eval  'NODEARRAY=($(sort -u $PBS_NODEFILE))'
TARGETS=(procon omp_sec_procon omp_parallel_procon)

# PBS script gets executed in node home directory, even on master node
# also the home directory is copied everywhere, so there is no need to copy binary files
for NODE in "${NODEARRAY[@]}"; do
ssh "$NODE" 'bash -s' << 'ENDSSH'
  cd   ~/cluster-computing-up/blatt4/aufgabe_4_3
  
  echo procon
  /usr/bin/time -p ./procon
  sleep 5
  echo omp_sec_procon
  /usr/bin/time -p ./omp_sec_procon
  sleep 5
  echo omp_parallel_procon
  /usr/bin/time -p ./omp_parallel_procon
  sleep 5

ENDSSH
done

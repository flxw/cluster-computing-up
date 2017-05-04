#!/bin/bash

#NODES="913755.zusemaster.cvos.cluster-list_hostnames.sh.nodes"
NODES="$PBS_NODEFILE"

IFS=$'\r\n' GLOBIGNORE='*' command eval  'NODEARRAY=($(sort -u $NODES))'

echo " NODE    | UPTIME AND LOAD "
for node in "${NODEARRAY[@]}"; do
	echo -n "$node  |"
	ssh "$node" uptime
done

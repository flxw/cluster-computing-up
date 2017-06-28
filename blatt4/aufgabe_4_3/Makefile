CFLAGS=-Wall -fopenmp
TARGETS=procon omp_sec_procon omp_parallel_procon

.PHONY: all clean bench

all: $(TARGETS)

clean:
	rm -f $(TARGETS)

bench: $(TARGETS)
	for TARGET in $^; do /usr/bin/time -p ./$$TARGET; done

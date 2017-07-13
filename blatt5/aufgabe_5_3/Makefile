LDFLAGS = -lm -lrt -O2

all: montecarlo_pi omp_montecarlo_pi

montecarlo_pi: montecarlo_pi.c timing_utils.c
	$(CC) -o $@ $^ $(LDFLAGS) -lpthread

omp_montecarlo_pi: omp_montecarlo_pi.c timing_utils.c
	$(CC) -o $@ $^ $(LDFLAGS) -fopenmp

.PHONY: clean
clean:
	rm -f montecarlo_pi omp_montecarlo_pi

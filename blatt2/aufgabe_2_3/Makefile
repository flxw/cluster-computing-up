LDFLAGS = -lpthread -lm

all: montecarlo_pi affine_montecarlo_pi

montecarlo_pi: montecarlo_pi.c
	$(CC) -o $@ $^ $(LDFLAGS)

affine_montecarlo_pi: affine_montecarlo_pi.c
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f montecarlo_pi affine_montecarlo_pi

CC=gcc
CFLAGS=-O2 -fopenmp
LDFLAGS=-lcrypto

.PHONY: clean

caseq: caseq.c random.c md5tool.c
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

omp_caseq: omp_caseq.c random.c md5tool.c
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

all: caseq omp_caseq

clean:
	rm -rf *.o
	rm -rf caseq

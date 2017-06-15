#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <omp.h>

#include "random.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: %s samples threads\n", argv[0]);
    printf("  samples: samples to run in total\n");
    printf("  threads: number of threads to run the samples in\n");
    return 1;
  }

  unsigned int samples = atoi(argv[1]);
  unsigned int threads = atoi(argv[2]);

  unsigned int chunk_size = (unsigned int)ceil((double)samples / (double)threads);
  unsigned int n_inside = 0;

  // Approximate PI...
  #pragma omp parallel reduction(+ : n_inside) num_threads(threads)
  {
    int thread_num = omp_get_thread_num();
    int rand_state = thread_num;

    unsigned int rest = (samples - thread_num*chunk_size);
    unsigned int private_chunk_size = rest < chunk_size ? rest : chunk_size;

    double x, y, r;
    for(unsigned int i = 0; i < private_chunk_size; i++) {
        x = pr_random_f_local(1.0, &rand_state);
        y = pr_random_f_local(1.0, &rand_state);
        r = x*x + y*y;
        if(r<=1.0) n_inside++;
    }
  }

  double approx_pi = ((double)n_inside/(double)samples)*4.0;
  double relative_error = (approx_pi - M_PI) / M_PI;
  printf("%i\t%i\t%f\t%f", n_inside, samples-n_inside, approx_pi, relative_error);

  return EXIT_SUCCESS;
}

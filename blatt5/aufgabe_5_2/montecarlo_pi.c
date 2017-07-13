#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>


#include "random.h"
#include "timing_utils.h"

typedef struct _thread_data_t {
  unsigned int tid;
  unsigned int samples;
  unsigned int n_inside;
  unsigned int n_outside;
} thread_data_t;

/* thread function */
void *thr_func(void *arg) {
  thread_data_t *data = (thread_data_t *) arg;
  data->n_outside = data->n_inside = 0;

  for (;data->samples != 0; --data->samples) {
    double x = pr_random_f(1), y = pr_random_f(1);

    double r2 = x*x + y*y;

    if (r2 < 1) {
      data->n_inside++;
    } else {
      data->n_outside++;
    }
  }

  pthread_exit(NULL);
}

int main(int argc, char **argv) {
  // NEW: Save start time point of program.
  // Note: We use the monotonic clock.
  struct timespec start;
  clock_gettime(CLOCK_MONOTONIC, &start);

  if (argc != 3) {
    printf("Usage: %s samples threads\n", argv[0]);
    printf("  samples: samples to run in total\n");
    printf("  threads: number of threads to run the samples in\n");
    return 1;
  }

  unsigned int samples = atoi(argv[1]);
  unsigned int threads = atoi(argv[2]);

  pthread_t *thr = (pthread_t*) malloc(threads * sizeof(pthread_t));
  thread_data_t *thr_data = (thread_data_t*) malloc(threads * sizeof(thread_data_t));

  /* create threads */
  unsigned int i, rc;
  for (i = 0; i < threads; ++i) {
    thr_data[i].tid = i;

    if (i < threads - 1) {
      unsigned int s = samples / threads;
      thr_data[i].samples = s;
      samples -= s;
    } else {
      thr_data[i].samples = samples;
    }

    if ((rc = pthread_create(&thr[i], NULL, thr_func, &thr_data[i]))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      return EXIT_FAILURE;
    }
  }
  /* block until all thr, rceads complete */
  for (i = 0; i < threads; ++i) {
    pthread_join(thr[i], NULL);
  }

  // sum up all outside and insides
  unsigned int n_outside_total = 0;
  unsigned int n_inside_total  = 0;

  for (i = 0; i < threads; ++i) {
    n_outside_total += thr_data[i].n_outside;
    n_inside_total  += thr_data[i].n_inside;
  }

  double approx_pi = (4.0*(double)n_inside_total)/(double)(n_outside_total+n_inside_total);
  double relative_error = (approx_pi - M_PI) / M_PI;

  printf("%i\t%i\t%f\t%f", n_inside_total, n_outside_total, approx_pi, relative_error);
  // printf("Inside circle:  %i\n", n_inside_total);
  // printf("Outside circle: %i\n", n_outside_total);
  // printf("Pi approximation: %f\n", approx_pi);
  // printf("Relative error: %f\n", relative_error);


  struct timespec stop;
  clock_gettime(CLOCK_MONOTONIC, &stop);

  struct timespec elapsed;
  ts_diff(&start, &stop, &elapsed);
  printf("\nCalculation took %i milliseconds.", ts2ms(&elapsed));

  return EXIT_SUCCESS;
}

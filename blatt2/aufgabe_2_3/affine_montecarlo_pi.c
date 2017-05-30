#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <sched.h>

#include "random.h"

typedef struct _thread_data_t {
  unsigned int core_id;
  unsigned int tid;
  unsigned int samples;
  unsigned int n_inside;
  unsigned int n_outside;
} thread_data_t;

/* thread function */

int stick_this_thread_to_core(int core_id) {
   int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
   if (core_id < 0 || core_id >= num_cores)
      return EINVAL;

   cpu_set_t cpuset;
   CPU_ZERO(&cpuset);
   CPU_SET(core_id, &cpuset);

   pthread_t current_thread = pthread_self();
   return pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
}

void *thr_func(void *arg) {
  thread_data_t *data = (thread_data_t *) arg;
  stick_this_thread_to_core(data->core_id);
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
  if (argc != 3) {
    printf("Usage: %s samples threads\n", argv[0]);
    printf("  samples: samples to run in total\n");
    printf("  threads: number of threads to run the samples in\n");
    return 1;
  }

  unsigned int samples = atoi(argv[1]);
  unsigned int threads = atoi(argv[2]);
  unsigned int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
  unsigned int curr_core = 0;

  pthread_t *thr = (pthread_t*) malloc(threads * sizeof(pthread_t));
  thread_data_t *thr_data = (thread_data_t*) malloc(threads * sizeof(thread_data_t));

  /* create threads */
  unsigned int i, rc;
  for (i = 0; i < threads; ++i) {
    thr_data[i].tid = i;
    thr_data[i].core_id = curr_core++;
    curr_core = curr_core % num_cores;

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

  return EXIT_SUCCESS;
}

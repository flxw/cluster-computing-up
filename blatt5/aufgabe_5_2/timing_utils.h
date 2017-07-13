#ifndef TIMING_UTILS_H
#define TIMING_UTILS_H

#include <time.h>
#include <math.h>

void ts_diff(struct timespec *start, struct timespec *stop, struct timespec *result);

long ts2ms(struct timespec *ts);

#endif
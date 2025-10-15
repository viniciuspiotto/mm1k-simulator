#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <stdlib.h>

#include "queue.h"

typedef struct {
    double time;
    int index;
} Elapsed_Time;

double random_u();
double generate_time(double l);
void min_times(Elapsed_Time *result, int count, ...);

#endif
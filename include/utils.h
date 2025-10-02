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
void min_with_index(Elapsed_Time * current_elapsed_time, double a, int index_a, double b, int index_b);
void min_four(Elapsed_Time * current_elapsed_time, double a, double b, double c, double d);
void min_three(Elapsed_Time * current_elapsed_time, double a, double b, double c);

#endif
#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <stdlib.h>
#include "queue.h"

typedef struct {
    double time;
    int index;
} Elapsed_Time;

typedef struct {
    double througput;
    double average_response_time;
    double blocking_probability;
    double occupancy;
} GenericPerformanceMetrics;

typedef struct {
    double fairness;
    double recall;
    double precision;
} ProblemPerformanceMetrics;

double random_u();
double generate_time(double l);
void min_times(Elapsed_Time *result, int count, ...);
void progress(int max_value, int current_value);
float fairness_jain(float x[], int n) ;

#endif
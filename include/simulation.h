#ifndef SIMULATION_H
#define SIMULATION_H

#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#include "utils.h"
#include "queue.h"

typedef struct {
    double prev_time;
    unsigned long int qnt_persons;
    double sum_area;
} little_metric;



void simulation(unsigned int interation, double simulation_time, Queue * queues, double  service_time_avarage);
void new_little_metric(little_metric *metric);
void update_little_information(little_metric *lm, double elapsed_time, bool isIncrement);
void update_little_only_sum_area(little_metric *lm, double elapsed_time);

#endif
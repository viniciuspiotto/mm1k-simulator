#ifndef SIMULATION_H
#define SIMULATION_H

#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#include "utils.h"

typedef struct {
    double prev_time;
    unsigned long int qnt_persons;
    double sum_area;
} little_metric;

// change the params
void simulation(unsigned int interation, double simulation_time, double arrival_time_avarage, double  service_time_avarage, char result_metric_file_path[], char raw_metric_file_path[]);

void new_little_metric(little_metric *metric);
void update_little_information(little_metric *lm, double elapsed_time, bool isIncrement);
void update_little_only_sum_area(little_metric *lm, double elapsed_time);

#endif
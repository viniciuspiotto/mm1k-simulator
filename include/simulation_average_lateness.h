#ifndef SIMULATION_AVERAGE_LATENESS_H
#define SIMULATION_AVERAGE_LATENESS_H

#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#include "little.h"
#include "utils.h"
#include "queue.h"

typedef struct {
    unsigned int elements_in_window;
    double total_lateness_request_met;
    double total_requests_arrive_in_queue;
    unsigned int schedule_size, total_served;
    Queue queue;
} Metrics;

Metrics * start_metrics(int amount, int size);
void simulation_average_lateness(GenericPerformanceMetrics *genericPerformanceMetrics,
    ProblemPerformanceMetrics *problemPerformanceMetrics, double odds[3],
     double simulation_time, Queue * queues, Metrics * metrics, 
    double service_time_avarage, bool printOutput);

#endif
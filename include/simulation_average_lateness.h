#ifndef SIMULATION_AVERAGE_LATENESS_H
#define SIMULATION_AVERAGE_LATENESS_H

#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#include "little.h"
#include "utils.h"
#include "queue.h"
#include "max_heap.h"

typedef struct {
    unsigned int elements_in_window;
    double total_lateness_request_met;
    double total_requests_arrive_in_queue;
    unsigned int schedule_size;
} Metrics;

Metrics * start_metrics(int amount, int size);
void simulation_average_lateness(unsigned int interation, double simulation_time, Queue * queues, Metrics * metrics, double  service_time_avarage);

#endif
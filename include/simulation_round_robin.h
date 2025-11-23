#ifndef SIMULATION_ROUND_ROBIN_H
#define SIMULATION_ROUND_ROBIN_H

#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#include "utils.h"
#include "little.h"
#include "queue.h"

void simulation_round_robin(GenericPerformanceMetrics *genericPerformanceMetrics,
    ProblemPerformanceMetrics *problemPerformanceMetrics, double odds[3], 
    double simulation_time, 
    Queue * queues, double service_time_avarage, bool isProblem);

#endif
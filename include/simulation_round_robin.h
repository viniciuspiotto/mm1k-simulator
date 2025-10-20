#ifndef SIMULATION_ROUND_ROBIN_H
#define SIMULATION_ROUND_ROBIN_H

#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#include "utils.h"
#include "queue.h"

void simulation_round_robin(unsigned int interation, double simulation_time, Queue * queues, double service_time_avarage);

#endif
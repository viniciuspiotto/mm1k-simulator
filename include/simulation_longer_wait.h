#ifndef SIMULATION_LONGER_WAIT_H
#define SIMULATION_LONGER_WAIT_H

#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#include "utils.h"
#include "queue.h"
#include "min_heap.h"

void simulation_longer_wait(unsigned int interation, double simulation_time, Queue * queues, double  service_time_avarage);

#endif
#ifndef STATISTIC_H
#define STATISTIC_H

#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "utils.h"
#include "queue.h"
#include "simulation_longer_wait.h"
#include "simulation_round_robin.h"
#include "simulation_average_lateness.h"

void service_time_variation(double arrival_time_rate[3], int queue_size, double odds[3], char * arrival_time_name);
void queue_size_variation(double arrival_time_rate[3], int j, double service_time_rate, double odds[3], char * charts_name);
#endif
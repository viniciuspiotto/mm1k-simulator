#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "statistic.h"
#include "queue.h"
#include "simulation_longer_wait.h"
#include "simulation_round_robin.h"
#include "simulation_average_lateness.h"

int main() {

    srand(time(NULL));
    
    double arrival_time_rate[3][3] = {
        {0.5, 0.5, 0.5},  
        {0.3, 0.5, 1},  
        {0.7, 0.7, 2}   
    };

    char *arrival_time_rate_names[] = {
        "same_arrival_time_rate", 
        "low_to_high_arrival_time_rate", 
        "high_arrival_time_rate"
    };
    int queue_size = 100;
    double odds[] = {0.16, 0.38, 0.01};

    for(int i = 0; i < 3; i++){
        printf("\n\n\tSimulation to %s started:\n", arrival_time_rate_names[i]);
        service_time_variation(arrival_time_rate[i], queue_size, odds, arrival_time_rate_names[i]);
    }

    double arrival_time_rate_problem[] = {0.53, 0.37, 0.10};
    double service_time_rate = 1;
    char *charts_name[] = {
        "avarage_lateness", 
        "longer_Wait", 
        "round_robin"
    };

    for(int i = 0; i < 3; i++){
        printf("\n\n\tSimulation to %s started:\n", charts_name[i]);
        queue_size_variation(arrival_time_rate_problem, i, service_time_rate, odds, charts_name[i]);
    }

    printf("\n");

    return 0;
}


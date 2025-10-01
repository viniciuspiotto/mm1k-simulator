#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utils.h"
#include "queue.h"
#include "simulation.h"

const double SIMULATION_TIME = 86400.0; // 24 * 60 * 60 (24 hours)

int main() {

    srand(time(NULL));
    // make this a input

    const double SERVICE_TIME_RATE = 10.0;
    double service_time_avarage = 1.0 / SERVICE_TIME_RATE;
    //double occupation = 0.8; // 80%
    unsigned int n = 1; // number of rounds - is one only for tests
    // Different occupations should have different arrival time rates.
    double arrival_time_rate[3] = {service_time_avarage * 0.8, service_time_avarage * 0.9, service_time_avarage * 0.95};
    Queue * queues = start_queues(3, arrival_time_rate, 100);

    //printf("[INFO]: E[N] = %.3f | E[W] = %.3f\n", occupation/(1 - occupation), 1 / (service_time_avarage - arrival_time_rate));
    //printf("[INFO]: λ = %.3f | μ = %.3f\n", arrival_time_rate, service_time_avarage);
    
    for (unsigned int i = 1; i <= n; i++) {
        simulation(i, SIMULATION_TIME, queues, service_time_avarage);
    }
    printf("\n\tSimulation completed successfully!\n\n");
    return 0;
}
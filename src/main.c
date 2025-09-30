#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utils.h"
#include "simulation.h"

const double SIMULATION_TIME = 86400.0; // 24 * 60 * 60 (24 hours)

int main() {
    srand(time(NULL));

    // make this a input
    const double SERVICE_TIME_RATE = 10.0;
    
    double service_time_avarage = 1.0 / SERVICE_TIME_RATE;
    double occupation = 0.8; // 80%
    unsigned int n = 10; // number of rounds
    
    double arrival_time_rate = service_time_avarage * occupation;
    printf("[INFO]: E[N] = %.3f | E[W] = %.3f\n", occupation/(1 - occupation), 1 / (service_time_avarage - arrival_time_rate));
    printf("[INFO]: λ = %.3f | μ = %.3f\n", arrival_time_rate, service_time_avarage);
    
    for (int i = 1; i <= n; i++) {
        // think in pass the queues or all the arrived avarage time in params
        // simulation(i, SIMULATION_TIME, service_time_avarage);
    }
    
    return 0;
}
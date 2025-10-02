#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"
#include "queue.h"
#include "simulation.h"
#include "min_heap.h"

const double SIMULATION_TIME = 86400.0; // 24 * 60 * 60 (24 hours)
const unsigned int QUEUE_AMOUNT = 3;

int main() {

    srand(time(NULL));
    double service_time_rate;
    double arrival_time_rate[QUEUE_AMOUNT];
    unsigned int n;

    printf("--- Configuração da Simulação ---\n");
    
    printf("Digite a taxa de serviço (μ): ");
    scanf("%lf", &service_time_rate);
    
    const double service_time_avarage = 1.0 / service_time_rate;

    printf("Digite a taxa de chegada para a Fila 1 (λ1): ");
    scanf("%lf", &arrival_time_rate[0]);

    printf("Digite a taxa de chegada para a Fila 2 (λ2): ");
    scanf("%lf", &arrival_time_rate[1]);

    printf("Digite a taxa de chegada para a Fila 3 (λ3): ");
    scanf("%lf", &arrival_time_rate[2]);

    printf("Número de iterações (n): ");
    scanf("%u", &n);

    printf("----------------------------------\n\n");
    
    Queue * queues = start_queues(QUEUE_AMOUNT, arrival_time_rate, 100);

    for (unsigned int i = 1; i <= n; i++) {
        simulation(i, SIMULATION_TIME, queues, service_time_avarage);
    }

    free(queues);

    return 0;
}


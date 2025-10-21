#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "queue.h"
#include "simulation_longer_wait.h"
#include "simulation_round_robin.h"
#include "simulation_average_lateness.h"

const double SIMULATION_TIME = 86400.0; // 24 * 60 * 60 (24 hours)
const unsigned int QUEUE_AMOUNT = 3;

int main() {

    srand(time(NULL));
    double service_time_rate;
    double arrival_time_rate[QUEUE_AMOUNT];
    unsigned int n, queue_size, schedule_size;

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

    printf("Digite o tamanho das filas: ");
    scanf("%u", &queue_size);

    printf("Digite o tamanho da janela: ");
    scanf("%u", &schedule_size);

    printf("Número de iterações (n): ");
    scanf("%u", &n);

    printf("----------------------------------\n\n");
    
    for (unsigned int i = 1; i <= n; i++) {
        Queue * queues = start_queues(QUEUE_AMOUNT, arrival_time_rate, queue_size);
        Metrics * metrics = start_metrics(QUEUE_AMOUNT, schedule_size);
        simulation_average_lateness(i, SIMULATION_TIME, queues, metrics, service_time_avarage);
        free(queues);
        free(metrics);
    }

    return 0;
}


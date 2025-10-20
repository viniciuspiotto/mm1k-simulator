#include "simulation_round_robin.h"

#define DBL_MAX __DBL_MAX__

void simulation_round_robin(unsigned int interation, double simulation_time, Queue * queues, double service_time_avarage) {

    printf("\nInteração: [%d]\n", interation);
    
    double exit_time = DBL_MAX;

    int current_queue = 0;
    bool server_busy = false;

    Elapsed_Time current_elapsed_time;
    current_elapsed_time.time = 0.0;
    current_elapsed_time.index = -1;

    little_metric E_N;
    new_little_metric(&E_N);

    little_metric E_W_ARRIVAL;
    new_little_metric(&E_W_ARRIVAL);
    little_metric E_W_EXIT;
    new_little_metric(&E_W_EXIT);

    long total_departures = 0;
    
    double next_arrival_time[] = {
        generate_time(queues[0].arrival_time_avarage),
        generate_time(queues[1].arrival_time_avarage),
        generate_time(queues[2].arrival_time_avarage)
    };

    Element new_element;

    bool someone_arrived;
    
    while(current_elapsed_time.time <= simulation_time) {
        server_busy ?
            min_times(&current_elapsed_time, 4, next_arrival_time[0], next_arrival_time[1], next_arrival_time[2], exit_time) :
            min_times(&current_elapsed_time, 3, next_arrival_time[0], next_arrival_time[1], next_arrival_time[2]);

        someone_arrived = current_elapsed_time.index < 3;

        if (someone_arrived) {
            int queue_index = current_elapsed_time.index;
            
            new_element.arrival_time = current_elapsed_time.time;
            bool isInsert = insert(&queues[queue_index], new_element);

            next_arrival_time[queue_index] = current_elapsed_time.time + generate_time(queues[queue_index].arrival_time_avarage);

            if (isInsert) {
                if (!server_busy) {
                    server_busy = true;
                    dequeue(&queues[queue_index]);
                    exit_time = current_elapsed_time.time + generate_time(service_time_avarage);
                    current_queue = queue_index;
                }
                update_little_information(&E_N, current_elapsed_time.time, true);
                update_little_information(&E_W_ARRIVAL, current_elapsed_time.time, true);
            }
        } else {
            total_departures++;

            if (!is_empty(&queues[0]) || !is_empty(&queues[1]) || !is_empty(&queues[2])) {
                if(is_empty(&queues[current_queue])){
                    for(int i = 0; i < 3; i++){
                        current_queue = (current_queue + 1) % 3;
                        if(!is_empty(&queues[current_queue])){
                            break;
                        }
                    }
                }

                dequeue(&queues[current_queue]);
                exit_time = current_elapsed_time.time + generate_time(service_time_avarage);

                update_little_information(&E_N, current_elapsed_time.time, false);
                update_little_information(&E_W_EXIT, current_elapsed_time.time, true);
            } else {
                server_busy = false;
                exit_time = DBL_MAX;
            }
        }
    }

    E_W_ARRIVAL.sum_area += (current_elapsed_time.time - E_W_ARRIVAL.prev_time) * E_W_ARRIVAL.qnt_persons;
    E_W_EXIT.sum_area += (current_elapsed_time.time - E_W_EXIT.prev_time) * E_W_EXIT.qnt_persons;

    double E_N_FINAL = (E_N.sum_area) / current_elapsed_time.time;
    double E_W_FINAL = (E_W_ARRIVAL.sum_area - E_W_EXIT.sum_area) / E_W_ARRIVAL.qnt_persons;

    double lambda = E_W_ARRIVAL.qnt_persons/current_elapsed_time.time;

    printf("\nE_W persons: %ld current_elapsed_time.time: %.8f lambda: %.8f\n\n", E_W_ARRIVAL.qnt_persons, current_elapsed_time.time, lambda);

    double erro_little = E_N_FINAL - lambda * E_W_FINAL;

    printf("E[N]: %.3f\n", E_N_FINAL);
    printf("E[W]: %.3f\n", E_W_FINAL);
    printf("little: %.10f\n", erro_little);

}
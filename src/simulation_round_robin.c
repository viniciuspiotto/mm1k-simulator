#include "simulation.h"
#include <stdio.h>

#define DBL_MAX __DBL_MAX__

void simulation_round_robin(unsigned int interation, double simulation_time, Queue * queues, double service_time_avarage, double quantum) {

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
    Element element_in_service;
    double current_quantum = quantum;

    bool someone_arrived, quantum_finished;
    bool people_finished = true;
    
    while(current_elapsed_time.time <= simulation_time) {

        server_busy ?
            min_times(&current_elapsed_time, 5, next_arrival_time[0], next_arrival_time[1], next_arrival_time[2], exit_time, current_quantum) :
            min_times(&current_elapsed_time, 4, next_arrival_time[0], next_arrival_time[1], next_arrival_time[2], current_quantum);

        someone_arrived = current_elapsed_time.index < 3;
        quantum_finished = current_elapsed_time.index == 4;

        if (someone_arrived) {

            int queue_index = current_elapsed_time.index;
            
            new_element.arrival_time = current_elapsed_time.time;
            new_element.exit_time = -1; 
            new_element.time_in_service = -1;

            bool isInsert = false;
            
            if (server_busy) {
                isInsert = insert(&queues[queue_index], new_element);
            } else {

                server_busy = true;
                double gerated_time = generate_time(service_time_avarage);

                exit_time = current_elapsed_time.time + gerated_time;
                
                element_in_service.arrival_time = current_elapsed_time.time;
                element_in_service.exit_time = exit_time; 
                element_in_service.time_in_service = gerated_time;

                current_queue = current_elapsed_time.index;
            }

            next_arrival_time[queue_index] = current_elapsed_time.time + generate_time(queues[queue_index].arrival_time_avarage);
        
            if (isInsert) {
                update_little_information(&E_N, current_elapsed_time.time, true);
                update_little_information(&E_W_ARRIVAL, current_elapsed_time.time, true);
            }

        } else {
            
            people_finished = true;

            if (quantum_finished) { 


                if (element_in_service.exit_time > current_elapsed_time.time) {
                    element_in_service.time_in_service = element_in_service.exit_time - current_elapsed_time.time;
                    insert(&queues[current_queue], element_in_service);
                    people_finished = false;
                } 
                
                for(int i = 0; i < 3; i++){
                    current_queue = (current_queue + 1) % 3;
                    if(!is_empty(&queues[current_queue])){
                        break;
                    }
                }

                current_quantum += quantum;
            }

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

                Element * element_dequeded = dequeue_with_return(&queues[current_queue]);
                element_in_service.arrival_time = (*element_dequeded).arrival_time;

                if((*element_dequeded).exit_time != -1){
                    exit_time = current_elapsed_time.time + (*element_dequeded).exit_time;
                } else {
                    exit_time = current_elapsed_time.time + generate_time(service_time_avarage);
                }

                element_in_service.exit_time = exit_time; 
                element_in_service.time_in_service = exit_time - current_elapsed_time.time;
                
                if (people_finished) {
                    update_little_information(&E_N, current_elapsed_time.time, false);
                    update_little_information(&E_W_EXIT, current_elapsed_time.time, true);
                }
                
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
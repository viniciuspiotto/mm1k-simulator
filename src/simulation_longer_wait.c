#include "simulation_longer_wait.h"

#define DBL_MAX __DBL_MAX__

void new_little_metric(little_metric *lm) {
    lm->prev_time = 0.0;
    lm->qnt_persons = 0;
    lm->sum_area = 0.0;
}

void update_little_information(little_metric *lm, double elapsed_time, bool isIncrement) {
    lm->sum_area += (elapsed_time - lm->prev_time) * lm->qnt_persons;
    lm->qnt_persons = (isIncrement) 
    ? lm->qnt_persons + 1 
    : lm->qnt_persons - 1;
    lm->prev_time = elapsed_time;
}

void update_little_only_sum_area(little_metric *lm, double elapsed_time) {
    lm->sum_area += (elapsed_time - lm->prev_time) * lm->qnt_persons;
    lm->prev_time = elapsed_time;
}

void simulation_longer_wait(unsigned int interation, double simulation_time, Queue * queues, double service_time_avarage) {

    printf("\nInteração: [%d]\n", interation);
    
    double exit_time = DBL_MAX;
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
    
    MinHeap* min_heap = create_min_heap(3);

    double next_arrival_time[] = {
        generate_time(queues[0].arrival_time_avarage),
        generate_time(queues[1].arrival_time_avarage),
        generate_time(queues[2].arrival_time_avarage)
    };

    Element new_element;

    while(current_elapsed_time.time <= simulation_time) {
        
        server_busy ? 
            min_times(&current_elapsed_time, 4, next_arrival_time[0], next_arrival_time[1], next_arrival_time[2], exit_time) :
            min_times(&current_elapsed_time, 3, next_arrival_time[0], next_arrival_time[1], next_arrival_time[2]);

        printf("%d %f\n", current_elapsed_time.index, current_elapsed_time.time);

        if (current_elapsed_time.index != 3) {
            
            int queue_index = current_elapsed_time.index;

            new_element.arrival_time = current_elapsed_time.time;
            
            bool isInsert = insert(&queues[queue_index], new_element);
            
            if (server_busy && queues[queue_index].current_size == 1) {
                NodeMin* new_node = (NodeMin*)malloc(sizeof(NodeMin));
                new_node->index = queue_index;
                new_node->arrived_time = current_elapsed_time.time;
                insert_min_node(min_heap, new_node);
            } else if (!server_busy) {
                server_busy = true;
                dequeue(&queues[queue_index]);
                exit_time = current_elapsed_time.time + generate_time(service_time_avarage);
            }

            next_arrival_time[queue_index] = current_elapsed_time.time + generate_time(queues[queue_index].arrival_time_avarage);
        
            if (isInsert) {
                update_little_information(&E_N, current_elapsed_time.time, true);
                update_little_information(&E_W_ARRIVAL, current_elapsed_time.time, true);   
            }

        } else {
            
            total_departures++;

            if (min_heap->size > 0) {
                
                int next_queue_index = extract_min(min_heap);
            
                dequeue(&queues[next_queue_index]);

                exit_time = current_elapsed_time.time + generate_time(service_time_avarage);
                
                if (!is_empty(&queues[next_queue_index])) {
                    Element next_person = get_first(&queues[next_queue_index]);
                    
                    NodeMin* new_candidate_node = (NodeMin*)malloc(sizeof(NodeMin));
                    new_candidate_node->index = next_queue_index;
                    new_candidate_node->arrived_time = next_person.arrival_time;
                    insert_min_node(min_heap, new_candidate_node);
                }

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
    printf("little: %.3f\n", erro_little);

    free_min_heap(min_heap);
}
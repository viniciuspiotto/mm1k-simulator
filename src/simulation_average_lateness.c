#include "simulation_average_lateness.h"

#define DBL_MAX __DBL_MAX__

void start_metric (Metrics * metric, int size){
    metric->schedule_size = size;
    metric->elements_in_window = 0;
    metric->total_lateness_request_met = 0.0;
    metric->total_requests_arrive_in_queue = 0.0;
    start_queue(&(metric->queue), metric->schedule_size, 0);
}

Metrics * start_metrics(int amount, int size) {
    Metrics * metrics = malloc(sizeof(Metrics) * amount);
    for(int i = 0; i < amount; i++){
        start_metric(&metrics[i], size);
    }
    return metrics;
}

void slide_window(Metrics * metric, double time, bool isArrival){

    Element * element_dequeded = dequeue(&(metric->queue));
    metric->elements_in_window--;
    metric->total_requests_arrive_in_queue -= element_dequeded->arrival_time;

    if(isArrival){
        metric->total_lateness_request_met += (time - element_dequeded->arrival_time);
    }
    
}

void simulation_average_lateness(unsigned int interation, double simulation_time, Queue * queues, Metrics * metrics, double service_time_avarage) {

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

    MaxHeap* max_heap = create_max_heap(3);

    bool someone_arrived;
    
    while(current_elapsed_time.time <= simulation_time) {

        server_busy ?
            min_times(&current_elapsed_time, 4, next_arrival_time[0], next_arrival_time[1], next_arrival_time[2], exit_time) :
            min_times(&current_elapsed_time, 3, next_arrival_time[0], next_arrival_time[1], next_arrival_time[2]);

        someone_arrived = current_elapsed_time.index != 3;

        if (someone_arrived) {

            int queue_index = current_elapsed_time.index;
            
            new_element.arrival_time = current_elapsed_time.time;
            bool isInsert = insert(&queues[queue_index], new_element);

            next_arrival_time[queue_index] = current_elapsed_time.time + generate_time(queues[queue_index].arrival_time_avarage);

            if (isInsert) {

                update_little_information(&E_N, current_elapsed_time.time, true);
                update_little_information(&E_W_ARRIVAL, current_elapsed_time.time, true);

                if (metrics[queue_index].elements_in_window >= metrics[queue_index].schedule_size) {
                    slide_window(&metrics[queue_index], current_elapsed_time.time, false);
                }
                
                metrics[queue_index].total_requests_arrive_in_queue += current_elapsed_time.time;
                metrics[queue_index].elements_in_window++;
                insert(&metrics[queue_index].queue, new_element);
                
               if (!server_busy) {
                    server_busy = true;
                    current_queue = queue_index;
                    exit_time = current_elapsed_time.time + generate_time(service_time_avarage);
               } else if (queues[queue_index].current_size == 1) {
                    NodeMax* new_node = (NodeMax*)malloc(sizeof(NodeMax));
                    new_node->index = queue_index;
                    new_node->average_lateness = 0.0;
                    insert_max_node(max_heap, new_node);
                }
            }

        } else {

            total_departures++;

            slide_window(&metrics[current_queue], current_elapsed_time.time, true);
            dequeue(&queues[current_queue]);

            update_little_information(&E_N, current_elapsed_time.time, false);
            update_little_information(&E_W_EXIT, current_elapsed_time.time, true);

            if (!is_empty(&queues[current_queue])) {
                NodeMax* candidate_node = (NodeMax*)malloc(sizeof(NodeMax));
                candidate_node->index = current_queue;
                
                if (metrics[current_queue].elements_in_window > 0) {
                    candidate_node->average_lateness = (1.0 / metrics[current_queue].elements_in_window) * (queues[current_queue].current_size * current_elapsed_time.time - metrics[current_queue].total_requests_arrive_in_queue + metrics[current_queue].total_lateness_request_met);
                } else {
                    candidate_node->average_lateness = 0.0;
                }
                insert_max_node(max_heap, candidate_node);
            }

            if (max_heap->size > 0) {
                int next_queue_index = extract_max(max_heap);
                current_queue = next_queue_index; 
                exit_time = current_elapsed_time.time + generate_time(service_time_avarage);
            } else {
                server_busy = false;
                exit_time = DBL_MAX;
                current_queue = -1;
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
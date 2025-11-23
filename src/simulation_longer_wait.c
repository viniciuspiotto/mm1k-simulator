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

void simulation_longer_wait(GenericPerformanceMetrics *genericPerformanceMetrics,
    ProblemPerformanceMetrics *problemPerformanceMetrics, double odds[3], double simulation_time, 
    Queue * queues, double service_time_avarage) {

    float average_service_time = 0;
    unsigned int arrivals = 0;
    unsigned int blockeds = 0;
    unsigned int positive_served = 0; 
    unsigned int positive_arrival = 0;
    unsigned int total_served[] = {0,0,0};
    
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

        if (current_elapsed_time.index != 3) {

            arrivals++;
            
            int queue_index = current_elapsed_time.index;

            bool isPositive = (rand() % 100) <= odds[queue_index];
            positive_arrival += isPositive ? 1 : 0;
            new_element.isPositive = isPositive;

            new_element.arrival_time = current_elapsed_time.time;
            
            bool isInsert = insert(&queues[queue_index], new_element);
            
            if (server_busy && queues[queue_index].current_size == 1) {
                NodeMin* new_node = (NodeMin*)malloc(sizeof(NodeMin));
                new_node->index = queue_index;
                new_node->arrived_time = current_elapsed_time.time;
                insert_min_node(min_heap, new_node);
            } else if (!server_busy) {
                server_busy = true;
                Element *element_dequeded = dequeue(&queues[queue_index]);
                exit_time = current_elapsed_time.time + generate_time(service_time_avarage);
                free(element_dequeded);
                element_dequeded = NULL;
            }

            next_arrival_time[queue_index] = current_elapsed_time.time + generate_time(queues[queue_index].arrival_time_avarage);
        
            if (isInsert) {
                update_little_information(&E_N, current_elapsed_time.time, true);
                update_little_information(&E_W_ARRIVAL, current_elapsed_time.time, true);   
            } else {
                blockeds++;
            }

        } else {
            
            total_departures++;

            if (min_heap->size > 0) {
                
                int next_queue_index = extract_min(min_heap);

                Element * element_dequeded = dequeue(&queues[next_queue_index]);
                positive_served += (*element_dequeded).isPositive ? 1 : 0;
                total_served[next_queue_index]++;

                exit_time = current_elapsed_time.time + generate_time(service_time_avarage);
                average_service_time += exit_time - (*element_dequeded).arrival_time;
                
                if (!is_empty(&queues[next_queue_index])) {
                    Element next_person = get_first(&queues[next_queue_index]);
                    
                    NodeMin* new_candidate_node = (NodeMin*)malloc(sizeof(NodeMin));
                    new_candidate_node->index = next_queue_index;
                    new_candidate_node->arrived_time = next_person.arrival_time;
                    insert_min_node(min_heap, new_candidate_node);
                }

                free(element_dequeded);
                element_dequeded = NULL;

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

    double erro_little = E_N_FINAL - lambda * E_W_FINAL;
    (void) erro_little;
    
    (*genericPerformanceMetrics).througput = total_departures / current_elapsed_time.time;
    (*genericPerformanceMetrics).average_response_time = average_service_time / total_departures;
    (*genericPerformanceMetrics).blocking_probability = (float)blockeds / (float)arrivals;

    float x[3] = {
        ((double)total_served[0]/current_elapsed_time.time) / queues[0].arrival_time_avarage,
        ((double)total_served[1]/current_elapsed_time.time) / queues[1].arrival_time_avarage,
        ((double)total_served[2]/current_elapsed_time.time) / queues[2].arrival_time_avarage
    };
        
    (*problemPerformanceMetrics).fairness = fairness_jain(x, 3);
    (*problemPerformanceMetrics).recall = (float)positive_served / (float)positive_arrival;
    (*problemPerformanceMetrics).precision = (float)positive_served / (float)total_departures;

    free_min_heap(min_heap);
}
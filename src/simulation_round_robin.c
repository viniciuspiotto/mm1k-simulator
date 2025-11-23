#include "simulation_round_robin.h"

#define DBL_MAX __DBL_MAX__

void simulation_round_robin(GenericPerformanceMetrics *genericPerformanceMetrics,
    ProblemPerformanceMetrics *problemPerformanceMetrics, double odds[3], double simulation_time, 
    Queue * queues, double service_time_avarage, bool isProblem) {

    float average_service_time = 0;
    unsigned int arrivals = 0;
    unsigned int blockeds = 0;
    unsigned int positive_served = 0; 
    unsigned int positive_arrival = 0;
    unsigned int total_served[] = {0,0,0};
    
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
    bool isLate = false, isNight = false;

    while(current_elapsed_time.time <= simulation_time) {

        if(isProblem){
            if(current_elapsed_time.time > 36000.0 && !isLate){
                isLate = true;
                queues[0].arrival_time_avarage=queues[0].arrival_time_avarage*2;
                queues[1].arrival_time_avarage=queues[1].arrival_time_avarage*2;
                queues[2].arrival_time_avarage=queues[2].arrival_time_avarage*2;
            }

            if(current_elapsed_time.time > 61200.0 && !isNight){
                isNight = true;
                queues[0].arrival_time_avarage=queues[0].arrival_time_avarage/2;
                queues[1].arrival_time_avarage=queues[1].arrival_time_avarage/2;
                queues[2].arrival_time_avarage=queues[2].arrival_time_avarage/2;
            }
        }

        server_busy ?
            min_times(&current_elapsed_time, 4, next_arrival_time[0], next_arrival_time[1], next_arrival_time[2], exit_time) :
            min_times(&current_elapsed_time, 3, next_arrival_time[0], next_arrival_time[1], next_arrival_time[2]);

        someone_arrived = current_elapsed_time.index < 3;

        if (someone_arrived) {

            arrivals++;

            int queue_index = current_elapsed_time.index;

            bool isPositive = ((double)rand() / RAND_MAX) < (odds[queue_index] / 100.0);
            positive_arrival += isPositive ? 1 : 0;
            
            new_element.arrival_time = current_elapsed_time.time;
            new_element.isPositive = isPositive;
            bool isInsert = insert(&queues[queue_index], new_element);
            
            next_arrival_time[queue_index] = current_elapsed_time.time + generate_time(queues[queue_index].arrival_time_avarage);

            if (isInsert) {
                if (!server_busy) {
                    server_busy = true;
                    Element * element_dequeded = dequeue(&queues[queue_index]);
                    exit_time = current_elapsed_time.time + generate_time(service_time_avarage);
                    current_queue = queue_index;
                    free(element_dequeded);
                    element_dequeded = NULL;
                }
                update_little_information(&E_N, current_elapsed_time.time, true);
                update_little_information(&E_W_ARRIVAL, current_elapsed_time.time, true);
            } else {
                blockeds++;
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

                total_served[current_queue]++;

                Element * element_dequeded = dequeue(&queues[current_queue]);
                positive_served += (*element_dequeded).isPositive ? 1 : 0;
                exit_time = current_elapsed_time.time + generate_time(service_time_avarage);
                average_service_time += exit_time - (*element_dequeded).arrival_time;

                update_little_information(&E_N, current_elapsed_time.time, false);
                update_little_information(&E_W_EXIT, current_elapsed_time.time, true);

                free(element_dequeded);
                element_dequeded = NULL;

                current_queue = (current_queue + 1) % 3;

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

}
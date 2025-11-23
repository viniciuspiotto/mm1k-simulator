#include "simulation_average_lateness.h"

#define DBL_MAX __DBL_MAX__

void start_metric (Metrics * metric, int size){
    metric->schedule_size = size;
    metric->elements_in_window = 0;
    metric->total_lateness_request_met = 0.0;
    metric->total_requests_arrive_in_queue = 0.0;

    metric->total_served = 0;
    start_queue(&(metric->queue), metric->schedule_size, 0);
}

void print_metric (Metrics * metric, int current_index, double time, Queue * queue) {

    printf("\n --- WINDOW %5d --- \n", current_index);
    printf("T: %5d\n", metric->elements_in_window);
    printf("D: %5.5f\n", metric->total_lateness_request_met);
    printf("S: %5.5f\n", metric->total_requests_arrive_in_queue);
    printf("EXITED: %d\n", metric->total_served);
    printf("FINAL VALUE: %5.5f\n", (1.0 / metric->elements_in_window) * (queue->current_size * time - metric->total_requests_arrive_in_queue + metric->total_lateness_request_met));

    printf("QUEUE: ");
    print_queue (&metric->queue);
    printf("\n");

}

Metrics * start_metrics(int amount, int size) {
    Metrics * metrics = malloc(sizeof(Metrics) * amount);
    for(int i = 0; i < amount; i++){
        start_metric(&metrics[i], size);
    }
    return metrics;
}

void slide_window(Metrics * metric, Element * element_dequeded, double time, bool isArrival){

    if(isArrival){
        metric->total_lateness_request_met += (time - element_dequeded->arrival_time);
        int index = (metric->queue.first + metric->total_served) % metric->queue.max_size;
        metric->queue.queue[index].delay = time - element_dequeded->arrival_time;
        metric->total_requests_arrive_in_queue -= element_dequeded->arrival_time;
    } else  {
        metric->elements_in_window--;
        if (metric->total_served > 0) {
            metric->total_lateness_request_met -= element_dequeded->delay;
            metric->total_served--;
        } else {
            metric->total_requests_arrive_in_queue -= element_dequeded->arrival_time;
        }
    }
}

void simulation_average_lateness(GenericPerformanceMetrics *genericPerformanceMetrics,
    ProblemPerformanceMetrics *problemPerformanceMetrics, double odds[3],
    double simulation_time, Queue * queues, Metrics * metrics, 
    double service_time_avarage, bool printOutput) {
    
    double exit_time = DBL_MAX;

    int current_queue = 0;
    bool server_busy = false;

    float average_service_time = 0;
    unsigned int arrivals = 0;
    unsigned int blockeds = 0;
    unsigned int positive_served = 0; 
    unsigned int positive_arrival = 0;
    unsigned int total_served_current[] = {0,0,0};

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

        someone_arrived = current_elapsed_time.index != 3;

        if (someone_arrived) {

            arrivals++;

            int queue_index = current_elapsed_time.index;
            bool isPositive = (rand() % 100) <= odds[queue_index];

            positive_arrival += isPositive ? 1 : 0;
            
            new_element.arrival_time = current_elapsed_time.time;
            new_element.isPositive = isPositive;
            bool isInsert = insert(&queues[queue_index], new_element);

            next_arrival_time[queue_index] = current_elapsed_time.time + generate_time(queues[queue_index].arrival_time_avarage);

            if (isInsert) {

                update_little_information(&E_N, current_elapsed_time.time, true);
                update_little_information(&E_W_ARRIVAL, current_elapsed_time.time, true);

                if (metrics[queue_index].elements_in_window >= metrics[queue_index].schedule_size) {
                    Element * element_dequeded = dequeue(&metrics[queue_index].queue);
                    slide_window(&metrics[queue_index], element_dequeded, current_elapsed_time.time, false);
                    free(element_dequeded);
                    element_dequeded = NULL;
                }

                metrics[queue_index].total_requests_arrive_in_queue += current_elapsed_time.time;
                metrics[queue_index].elements_in_window++;
                insert(&metrics[queue_index].queue, new_element);
                if (printOutput) print_metric(&metrics[queue_index], queue_index, current_elapsed_time.time, &queues[queue_index]);
                
               if (!server_busy) {
                    server_busy = true;
                    current_queue = queue_index;
                    exit_time = current_elapsed_time.time + generate_time(service_time_avarage);
                } 
            } else {
                blockeds++;
            }

        } else {

            total_departures++;

            Element * element_dequeded = dequeue(&queues[current_queue]);
            positive_served += (*element_dequeded).isPositive ? 1 : 0;
            slide_window(&metrics[current_queue], element_dequeded, current_elapsed_time.time, true);
            total_served_current[current_queue]++;
            metrics[current_queue].total_served++;

            if (printOutput) print_metric(&metrics[current_queue], current_queue, current_elapsed_time.time, &queues[current_queue]);

            int index_max_value = -1;
            float max_value = -1;
            float temp;
            for (int i = 0; i < 3; i++){
                if(metrics[i].elements_in_window > 0 && !is_empty(&queues[i])){
                    temp = (1.0 / metrics[i].elements_in_window) * (queues[i].current_size * current_elapsed_time.time - metrics[i].total_requests_arrive_in_queue + metrics[i].total_lateness_request_met);
                    if(temp > max_value){
                        max_value = temp;
                        index_max_value = i;
                    }
                }
            }

            if (index_max_value > -1) {
                current_queue = index_max_value; 
                exit_time = current_elapsed_time.time + generate_time(service_time_avarage);
                average_service_time += exit_time - (*element_dequeded).arrival_time;
            } else {
                server_busy = false;
                exit_time = DBL_MAX;
                current_queue = -1;
            }

            update_little_information(&E_N, current_elapsed_time.time, false);
            update_little_information(&E_W_EXIT, current_elapsed_time.time, true);
            free(element_dequeded);
            element_dequeded = NULL;

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
        ((double)total_served_current[0]/current_elapsed_time.time) / queues[0].arrival_time_avarage,
        ((double)total_served_current[1]/current_elapsed_time.time) / queues[1].arrival_time_avarage,
        ((double)total_served_current[2]/current_elapsed_time.time) / queues[2].arrival_time_avarage
    };

    (*problemPerformanceMetrics).fairness = fairness_jain(x, 3);
    (*problemPerformanceMetrics).recall = (float)positive_served / (float)positive_arrival;
    (*problemPerformanceMetrics).precision = (float)positive_served / (float)total_departures;

}
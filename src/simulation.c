#include "simulation.h"

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

void simulation(unsigned int interation, double simulation_time, Queue * queues, double  service_time_avarage) {

    printf("\n\tIteration: [%d]\n", interation);
    // unsigned int qtd_person = 0;
    // unsigned long int queue = 0, longest_queue = 0;
    //double timer = 10.0, 
    double exit_time = DBL_MAX;
    bool person_begin_served = false;

    Elapsed_Time current_elapsed_time;
    current_elapsed_time.time = 0.0;
    current_elapsed_time.index = -1;
    int highest_wait_person = 0;

    //little_metric E_N;
    //new_little_metric(&E_N);
    //little_metric E_W_ARRIVAL;
    //new_little_metric(&E_W_ARRIVAL);
    //little_metric E_W_EXIT;
    //new_little_metric(&E_W_EXIT);

    //double temporary_E_N = 0.0;
    //double temporary_E_W = 0.0;
    //double temporary_lambda = 0.0;

    // start first arrival
    double next_arrival_time[] = {generate_time(queues[0].arrival_time_avarage),
                                  generate_time(queues[1].arrival_time_avarage),
                                  generate_time(queues[2].arrival_time_avarage)};

    while(current_elapsed_time.time <= simulation_time) {

        // will the timer or the next arrival or exit happen first?
        person_begin_served ? 
            min_four(&current_elapsed_time, 
                next_arrival_time[0], 
                next_arrival_time[1], 
                next_arrival_time[2], 
                exit_time) :
            min_three(&current_elapsed_time,
                next_arrival_time[0], 
                next_arrival_time[1], 
                next_arrival_time[2]);

        if (current_elapsed_time.index != 3){

            if (!person_begin_served) {
                exit_time = current_elapsed_time.time + generate_time(service_time_avarage);
                person_begin_served = true;
            } else {
                Element new_element;
                new_element.arrival_time = current_elapsed_time.time;
                insert(&queues[current_elapsed_time.index], new_element);
                // TODO : returns boolean, but we don't use it for anything yet if we can't insert it
                next_arrival_time[current_elapsed_time.index] = 
                    current_elapsed_time.time + 
                    generate_time(queues[current_elapsed_time.index].arrival_time_avarage);

            }
            
            // TODO :  do littles law here
            // update_little_information(&E_N, current_elapsed_time.time, true);
            // update_little_information(&E_W_ARRIVAL, current_elapsed_time.time, true);

        } else {
            
            exit_time = DBL_MAX;

            if (queues[0].current_size || queues[1].current_size || queues[2].current_size) {
                person_begin_served = true;
                // TODO : returns the index of the queue from which I have to remove the element
                // TODO: Temporary function, with poor practices, used only for compilation !!!
                highest_wait_person = less_time(queues[0], queues[1], queues[2]);
                // TODO : returns boolean, but we don't use it for anything yet if we can't remove it
                dequeue(&queues[highest_wait_person]);
                exit_time = current_elapsed_time.time + generate_time(service_time_avarage);
            } else {
                person_begin_served = false;
            }

            // do littles law here
            // update_little_information(&E_N, current_elapsed_time.time, false);
            // update_little_information(&E_W_EXIT, current_elapsed_time.time, true);
            
        }
    }

    //E_W_ARRIVAL.sum_area += (elapsed_time - E_W_ARRIVAL.prev_time) * E_W_ARRIVAL.qnt_persons;
    //E_W_EXIT.sum_area += (elapsed_time - E_W_EXIT.prev_time) * E_W_EXIT.qnt_persons;

    //fclose(metric);
    //fclose(raw_metric);

}
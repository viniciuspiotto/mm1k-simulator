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

void simulation(unsigned int interation, double simulation_time, double arrival_time_avarage, double  service_time_avarage, char result_metric_file_path[], char raw_metric_file_path[]) {
    FILE * metric = fopen(result_metric_file_path, "w");
    FILE *raw_metric = fopen(raw_metric_file_path, "a+");

    fseek(raw_metric, 0, SEEK_END);
    long file_size = ftell(raw_metric);
    if (file_size == 0) {
        fprintf(raw_metric, "N,Final_E[N],Final_E[W]\n");
    }
    fseek(raw_metric, 0, SEEK_END);

    fprintf(metric, "Time,E[N],E[W],Error_little\n");

    unsigned int qtd_person = 0;
    double elapsed_time = 0.0;
    unsigned long int queue = 0;
    unsigned long int longest_queue = 0;
    double timer = 10.0;
    double exit_time = DBL_MAX;

    // little metrics
    little_metric E_N;
    new_little_metric(&E_N);
    little_metric E_W_ARRIVAL;
    new_little_metric(&E_W_ARRIVAL);
    little_metric E_W_EXIT;
    new_little_metric(&E_W_EXIT);

    double temporary_E_N = 0.0;
    double temporary_E_W = 0.0;
    double temporary_lambda = 0.0;

    // start first arrival
    double next_arrival_time = generate_time(service_time_avarage);
    qtd_person++;

    while(elapsed_time <= simulation_time) {
        // will the timer or the next arrival or exit happen first?
        elapsed_time = min(timer, min(next_arrival_time, exit_time));

        if (elapsed_time == timer) {
            update_little_only_sum_area(&E_N, elapsed_time);
            update_little_only_sum_area(&E_W_ARRIVAL, elapsed_time);
            update_little_only_sum_area(&E_W_EXIT, elapsed_time);

            temporary_E_N = E_N.sum_area / elapsed_time;
            temporary_E_W = E_W_ARRIVAL.qnt_persons != 0 ? (E_W_ARRIVAL.sum_area - E_W_EXIT.sum_area) / E_W_ARRIVAL.qnt_persons : 0.0;
            temporary_lambda = E_W_ARRIVAL.qnt_persons / elapsed_time;

            fprintf(metric, "%lf,%lf,%lf,%lf\n", elapsed_time, temporary_E_N, temporary_E_W, fabs(temporary_E_N - temporary_lambda * temporary_E_W));
            timer += 10;
        } else if(elapsed_time == next_arrival_time) { // arrived people
            // no one is in queue
            if (!hasServices) {
                exit_time = elapsed_time + generate_time(service_time_avarage);
            } else {
                // add new person to our respective time
            }

            next_arrival_time = elapsed_time + generate_time(arrival_time_avarage);

            update_little_information(&E_N, elapsed_time, true);
            update_little_information(&E_W_ARRIVAL, elapsed_time, true);
        } else { // exit people
            queue--;
            exit_time = DBL_MAX;

            if (queue1.current_size || queue2 || queue3) {
                hasServices = true;

                highest_wait_person = less_time(queue1[0].arrived_time, queue2, queue3);
                exit_time = elapsed_time + generate_time(service_time_avarage);
            } else {
                hasServices = false;
            }

            update_little_information(&E_N, elapsed_time, false);
            update_little_information(&E_W_EXIT, elapsed_time, true);
        }
    }

    E_W_ARRIVAL.sum_area += (elapsed_time - E_W_ARRIVAL.prev_time) * E_W_ARRIVAL.qnt_persons;
    E_W_EXIT.sum_area += (elapsed_time - E_W_EXIT.prev_time) * E_W_EXIT.qnt_persons;

    fprintf(raw_metric, "%d,%lf,%lf\n", interation, E_N.sum_area / elapsed_time, (E_W_ARRIVAL.sum_area - E_W_EXIT.sum_area) / E_W_ARRIVAL.qnt_persons);

    fclose(metric);
    fclose(raw_metric);
}
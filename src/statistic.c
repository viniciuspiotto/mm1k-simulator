#include "statistic.h"

const double SIMULATION_TIME = 86400.0;
const unsigned int QUEUE_AMOUNT = 3;

double metric_result_simulation(int simulation_type, int metric, double odds[3], Queue * queues, double service_time_avarage, bool printOutput, int window_size){
    
    GenericPerformanceMetrics genericPerformanceMetrics;
    ProblemPerformanceMetrics problemPerformanceMetrics; 

    switch (simulation_type) {
        case 0: {
            Metrics * metrics = start_metrics(QUEUE_AMOUNT, window_size);
            simulation_average_lateness(&genericPerformanceMetrics, &problemPerformanceMetrics, odds, SIMULATION_TIME, queues, metrics, service_time_avarage, printOutput);
            free(metrics);
            break;
        }
        case 1:
            simulation_longer_wait(&genericPerformanceMetrics, &problemPerformanceMetrics, odds, SIMULATION_TIME, queues, service_time_avarage);
            break;
        case 2:
            simulation_round_robin(&genericPerformanceMetrics, &problemPerformanceMetrics, odds, SIMULATION_TIME, queues, service_time_avarage);
            break;
    }

    switch (metric) {
        case 0:
            return genericPerformanceMetrics.througput;
        break;
        case 1:
            return genericPerformanceMetrics.average_response_time;
            break;
        case 2:
            return genericPerformanceMetrics.blocking_probability;
            break;
        case 3:
            return problemPerformanceMetrics.fairness;
            break;
        case 4:
            return problemPerformanceMetrics.recall;
            break;
        case 5:
            return problemPerformanceMetrics.precision;
            break;
    }
    return 0.0;
}

long quantity_samples (int simulation_type, int metric, double odds[3], double * arrival_time_rate, int queue_size, double service_time_avarage, bool printOutput, int window_size){

    double average = 0;
    int number_samples = 10;
    double samples[number_samples];
    double standard_deviation = 0;

    Queue * queues;

    for(int k = 0; k < number_samples; k++){
        queues = start_queues(QUEUE_AMOUNT, arrival_time_rate, queue_size);
        double metric_value = metric_result_simulation(simulation_type, metric, odds, queues, service_time_avarage, printOutput, window_size);
        free_queues(queues, QUEUE_AMOUNT);
        average += metric_value;
        samples[k] = metric_value;
    }

    average = average / number_samples;
    for(int k = 0; k < number_samples; k++){ 
        standard_deviation += (samples[k] - average)*(samples[k] - average);
    }
    standard_deviation = sqrt(standard_deviation / (number_samples - 1)); 

    float E = 0.05 * average;
    long value_return = ceil(pow((1.96 * standard_deviation) / E, 2));
    if (value_return < 1) value_return = 1;
    return value_return;
}

void service_time_variation(double arrival_time_rate[3], int queue_size, double odds[3], char * arrival_time_name){

    FILE * f;
    char nome_arquivo[500];
    double service_time_rate_min = 0.2;
    double service_time_rate_max = 3;
    int steps = 10;
    double step = (service_time_rate_max - service_time_rate_min) / (steps - 1);
    int quantity_samples_calculated = 0;

    char *metrics_names[] = {
        "througput", 
        "average_response_time", 
        "blocking_probability"
    };

    for (int y = 0; y < 3; y++){ 

        sprintf(nome_arquivo, "results/generic/%s/%s.csv", arrival_time_name, metrics_names[y]);
        f = fopen(nome_arquivo, "w");
        if(f == NULL){
            printf("Error to open the file!\n");
            return;
        }
        fprintf(f, "simulation_type,service_time_avarage,metric_value\n\n");
        printf("\n\tProgress to %s metric:\n", metrics_names[y]);

        for (int j = 0; j < 3; j++){ 

            double service_time_rate = service_time_rate_min;

            for(int i = 0; i < steps; i ++){ 

                double current_metric = 0;
                quantity_samples_calculated = quantity_samples(j, y, odds, arrival_time_rate, queue_size, service_time_rate, false, queue_size);

                for(int x = 0; x < quantity_samples_calculated; x++){
                    progress(
                            3 * steps * quantity_samples_calculated,
                            j * (steps * quantity_samples_calculated) +
                            i * quantity_samples_calculated +
                            x
                        );
                    Queue * queues = start_queues(QUEUE_AMOUNT, arrival_time_rate, queue_size);
                    current_metric += metric_result_simulation(j, y, odds, queues, service_time_rate, false, queue_size);
                    free_queues(queues, QUEUE_AMOUNT);
                }

                current_metric = current_metric / quantity_samples_calculated;
                fprintf(f, "%d,%0.10f,%0.10f\n", j, service_time_rate, current_metric);
                service_time_rate += step;
                
            }
            
        }

        fclose(f);

    }

}

void queue_size_variation(double arrival_time_rate[3], int j, double service_time_rate, double odds[3], char * charts_name){

    FILE * f;
    char nome_arquivo[500];
    int queue_size_min = 10;
    int queue_size_max = 1000;
    int steps = 10;
    int step = (queue_size_max - queue_size_min) / (steps - 1);
    int queue_size = queue_size_min;
    int quantity_samples_calculated = 0;

    char *metrics_names[] = {
        "fairness", 
        "recall", 
        "precision"
    };

    for (int y = 3; y < 6; y++){ 

        sprintf(nome_arquivo, "results/problem/%s/%s.csv", charts_name, metrics_names[y-3]);
        f = fopen(nome_arquivo, "w");
        if(f == NULL){
            printf("Error to open the file!\n");
            return;
        }
        if (j != 0){
            fprintf(f, "queue_size,metric_value\n\n");
        } else {
            fprintf(f, "queue_size,window_size,metric_value\n\n");
        }

        printf("\n\tProgress to %s metric:\n", metrics_names[y-3]);
        queue_size = queue_size_min; 

        for(int i = 0; i < steps; i ++){
            
            if (j != 0) {

                double current_metric = 0;
                quantity_samples_calculated = quantity_samples(j, y, odds, arrival_time_rate, queue_size, service_time_rate, false, queue_size);
            
                for(int x = 0; x < quantity_samples_calculated; x++){
                    progress(steps * quantity_samples_calculated, i * quantity_samples_calculated + x);
                    Queue * queues = start_queues(QUEUE_AMOUNT, arrival_time_rate, queue_size);
                    current_metric += metric_result_simulation(j, y, odds, queues, service_time_rate, false, queue_size);
                    free_queues(queues, QUEUE_AMOUNT);
                }

                current_metric = current_metric / quantity_samples_calculated;
                fprintf(f, "%d,%0.10f\n", queue_size, current_metric);
            
            } else {

                for (int t = 0; t < 4; t++){

                    double current_metric = 0;
                    int current_window_size = queue_size;
                    if (t != 0) current_window_size = queue_size*pow(2,t);
                    quantity_samples_calculated = quantity_samples(j, y, odds, arrival_time_rate, queue_size, service_time_rate, false, current_window_size);
                    
                    for(int x = 0; x < quantity_samples_calculated; x++){
                        progress(steps * quantity_samples_calculated, i * quantity_samples_calculated + x);
                        Queue * queues = start_queues(QUEUE_AMOUNT, arrival_time_rate, queue_size);
                        current_metric += metric_result_simulation(j, y, odds, queues, service_time_rate, false, queue_size);
                        free_queues(queues, QUEUE_AMOUNT);
                    }

                    current_metric = current_metric / quantity_samples_calculated;
                    fprintf(f, "%d,%d,%0.10f\n", queue_size, current_window_size, current_metric);

                }
            }
        
            queue_size += step;

        }

        fclose(f);

    }

}



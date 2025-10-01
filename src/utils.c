#include <string.h>
#include "utils.h"

#include "simulation.h"

#define	RAND_MAX 2147483647

double random_u() {
    double u = rand() / ((double) RAND_MAX + 1);
    return 1.0 - u;
}

double generate_time(double l) {
    return (-1.0 / l) * log(random_u());
}

double min (double a, double b) {
    return (a < b) ? a : b;
}

// TODO: Temporary function, with poor practices, used only for compilation. !!!
int less_time(Queue a, Queue b, Queue c){
    double a_value = 100000000, b_value = 100000000, c_value = 1000000000;

    if (a.current_size > 0) a_value = a.queue[0].arrival_time;
    if (b.current_size > 0) b_value = b.queue[0].arrival_time;
    if (c.current_size > 0) c_value = c.queue[0].arrival_time;

    double min_value = min(c_value,min(a_value,b_value));

    if (min_value == a_value) return 0;
    if (min_value == b_value) return 1;

    return 2;

}

void min_with_index (Elapsed_Time * current_elapsed_time, double a, int index_a, double b, int index_b){
   current_elapsed_time->time  = (a < b) ? a : b;
   current_elapsed_time->index = (a < b) ? index_a : index_b;
}

void min_four (Elapsed_Time * current_elapsed_time, double a, double b, double c, double d){
    min_with_index(current_elapsed_time, a, 0, b, 1);
    min_with_index(current_elapsed_time, current_elapsed_time->time, current_elapsed_time->index, c, 2);
    min_with_index(current_elapsed_time, current_elapsed_time->time, current_elapsed_time->index, d, 3);
}

void min_three (Elapsed_Time * current_elapsed_time, double a, double b, double c){
    min_with_index(current_elapsed_time, a, 0, b, 1);
    min_with_index(current_elapsed_time, current_elapsed_time->time, current_elapsed_time->index, c, 2);
}
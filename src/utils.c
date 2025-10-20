#include <string.h>
#include <stdarg.h>

#include "utils.h"

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

void min_times(Elapsed_Time *result, int count, ...) {
    
    if (count <= 0) {
        result->time = 0.0;
        result->index = -1;
        return;
    }

    va_list args;
    va_start(args, count);

    double min_time = va_arg(args, double);
    result->time = min_time;
    result->index = 0;

    for (int i = 1; i < count; i++) {
        double current_time = va_arg(args, double);
        if (current_time < result->time) {
            result->time = current_time;
            result->index = i;
        }
    }

    va_end(args);
}
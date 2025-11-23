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

void progress(int max_value, int current_value) {

    float percent = (float)current_value / max_value * 100.0;
    int prints = (int)(percent / 10.0);

    printf("\r\t[");

    for (int i = 0; i < prints; i++) {
        printf("=");
    }
    for (int j = 0; j < 10 - prints; j++) {
        printf("-");
    }

    printf("] %.0f%%", percent);
    fflush(stdout);
}

float fairness_jain(float x[], int n) {
    float sum = 0.0f, sum_sq = 0.0f;
    for (int i = 0; i < n; i++) {
        sum += x[i];
        sum_sq += x[i] * x[i];
    }
    return (sum * sum) / (n * sum_sq);
}
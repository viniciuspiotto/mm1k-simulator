#include "utils.h"
#include <string.h>

#define	RAND_MAX 2147483647

double random_u() {
    double u = rand() / ((double) RAND_MAX + 1);
    return 1.0 - u;
}

double generate_time(double l) {
    return (-1.0 / l) * log(random_u());
}

double min(double a, double b) {
    return (a < b) ? a : b;
}
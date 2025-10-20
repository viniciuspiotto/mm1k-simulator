#ifndef LITTLE_H
#define LITTLE_H

typedef struct {
    double prev_time;
    unsigned long int qnt_persons;
    double sum_area;
} little_metric;

void new_little_metric(little_metric *lm);
void update_little_information(little_metric *lm, double elapsed_time, bool isIncrement);
void update_little_only_sum_area(little_metric *lm, double elapsed_time);

#endif
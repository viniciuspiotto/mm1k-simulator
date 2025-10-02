#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    double arrival_time;
} Element;

typedef struct {
    int max_size, current_size, first, last, qnt_persons;
    double arrival_time_avarage;
    Element * queue;
} Queue;

bool is_empty(Queue * queue);
bool insert(Queue * queue, Element element);
void dequeue(Queue * queue);
Element get_first(Queue * queue);
Queue * start_queues (int amount, double * arrivals_time_avarage, int size);

#endif
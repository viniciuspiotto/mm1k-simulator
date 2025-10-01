#include "queue.h"

bool is_full(Queue * queue){
    return (queue->current_size == queue->max_size);
}

bool is_empty(Queue * queue){
    return (queue->current_size == 0);
}

void start_queue (Queue * queue, int size, double arrival_time_avarage){
    queue->max_size = size;
    queue->current_size = 0;
    queue->arrival_time_avarage = arrival_time_avarage;
    queue->first = 0;
    queue->last = -1;
    queue->qnt_persons = 0;
    queue->queue = (Element*)malloc(size * sizeof(Element));
}

bool insert(Queue * queue, Element element){
    if (!is_full(queue)) {
        queue->last = (queue->last + 1) % queue->max_size;
        queue->queue[queue->last] = element;
        queue->current_size++;
        return true;
    } else {
        return false;
    }
}

bool dequeue (Queue * queue){
    if(is_empty(queue)){
        return false;
    } else {
        queue->first = (queue->first + 1) % queue->max_size;
        queue->current_size--;
        return true;
    }
}

Queue * start_queues (int amount, double * arrivals_time_avarage, int size){
    Queue * queues = malloc(sizeof(Queue) * amount);
    for(int i = 0; i < amount; i++){
        start_queue(&queues[i], size, arrivals_time_avarage[i]);
    }
    return queues;
}

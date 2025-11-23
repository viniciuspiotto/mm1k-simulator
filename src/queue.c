#include "queue.h"

bool is_full(Queue * queue){
    return (queue->current_size == queue->max_size);
}

bool is_empty (Queue * queue){
    return (queue->current_size == 0);
}

void print_queue(Queue * queue) {
    for (int i = 0; i < queue->current_size; i++) {
        int index = (queue->first + i) % queue->max_size;
        printf(" [%.4f, %.4f] ", queue->queue[index].arrival_time, queue->queue[index].delay);
    }
    printf("\n");
}

void start_queue (Queue * queue, int size, double arrival_time_rate){
    queue->max_size = size;
    queue->current_size = 0;
    queue->arrival_time_avarage = arrival_time_rate;
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
    }
    return false;
}

Element * dequeue(Queue * queue) {
    if (!is_empty(queue)) {

        Element * element_dequeue = malloc(sizeof(Element));

        element_dequeue->arrival_time = queue->queue[queue->first].arrival_time;
        element_dequeue->delay = queue->queue[queue->first].delay;
        element_dequeue->isPositive = queue->queue[queue->first].isPositive;

        queue->first = (queue->first + 1) % queue->max_size;
        queue->current_size--; 
        
        return element_dequeue;
    }
    return NULL;
}

Element get_first(Queue * queue) {
    return queue->queue[queue->first];
}

Queue * start_queues (int amount, double * arrivals_time_avarage, int size){
    Queue * queues = malloc(sizeof(Queue) * amount);
    for(int i = 0; i < amount; i++){
        start_queue(&queues[i], size, arrivals_time_avarage[i]);
    }
    return queues;
}

void free_queue(Queue *queue) {
    free(queue->queue);
}

void free_queues(Queue *queues, int amount) {
    for (int i = 0; i < amount; i++) {
        free_queue(&queues[i]);
    }
    free(queues);
}

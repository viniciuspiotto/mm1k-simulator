#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

typedef struct {
    unsigned int index;
    double arrived_time;
} NodeMin;

typedef struct {
    NodeMin **arr;
    int capacity;
    int size;
} MinHeap;

MinHeap* create_min_heap(int capacity);
void insert_min_node(MinHeap* minHeap, NodeMin* node);
int extract_min(MinHeap* minHeap);
void free_min_heap(MinHeap* minHeap);

#endif
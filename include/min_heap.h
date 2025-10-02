#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

typedef struct {
    unsigned int index;
    double arrived_time;
} Node;

typedef struct {
    Node **arr;
    int capacity;
    int size;
} MinHeap;

MinHeap* create_min_heap(int capacity);
void insert_node(MinHeap* minHeap, Node* node);
int extract_min(MinHeap* minHeap);

#endif
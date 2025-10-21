#ifndef MAX_HEAP_H
#define MAX_HEAP_H

typedef struct NodeMax {
    double average_lateness;
    int index;
} NodeMax;

typedef struct MaxHeap {
    int capacity;
    int size;
    NodeMax** arr;
} MaxHeap;

MaxHeap* create_max_heap(int capacity);
void insert_max_node(MaxHeap* maxHeap, NodeMax* node);
int extract_max(MaxHeap* maxHeap);
void free_max_heap(MaxHeap* maxHeap);

#endif
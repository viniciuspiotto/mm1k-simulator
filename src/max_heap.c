#include "max_heap.h"
#include <stdlib.h>

static void swap(NodeMax **a, NodeMax **b) {
    NodeMax *temp = *a;
    *a = *b;
    *b = temp;
}

MaxHeap* create_max_heap(int capacity) {
    MaxHeap* maxHeap = (MaxHeap*)malloc(sizeof(MaxHeap));
    maxHeap->capacity = capacity;
    maxHeap->size = 0;
    maxHeap->arr = (NodeMax**)malloc(capacity * sizeof(NodeMax*));
    return maxHeap;
}

static int parent(int i) {
    return (i - 1) / 2;
}

static int left_child(int i) {
    return (2 * i + 1);
}

static int right_child(int i) {
    return (2 * i + 2);
}

static void max_heapify(MaxHeap* maxHeap, int i) {
    int l = left_child(i);
    int r = right_child(i);
    int largest = i;

    if (l < maxHeap->size && maxHeap->arr[l]->average_lateness > maxHeap->arr[largest]->average_lateness) {
        largest = l;
    }

    if (r < maxHeap->size && maxHeap->arr[r]->average_lateness > maxHeap->arr[largest]->average_lateness) {
        largest = r;
    }

    if (largest != i) {
        swap(&maxHeap->arr[i], &maxHeap->arr[largest]);
        max_heapify(maxHeap, largest);
    }
}

void insert_max_node(MaxHeap* maxHeap, NodeMax* node) {
    if (maxHeap->size == maxHeap->capacity) {
        return;
    }

    maxHeap->size++;
    int i = maxHeap->size - 1;
    maxHeap->arr[i] = node;

    while (i != 0 && maxHeap->arr[parent(i)]->average_lateness < maxHeap->arr[i]->average_lateness) {
        swap(&maxHeap->arr[i], &maxHeap->arr[parent(i)]);
        i = parent(i);
    }
}

int extract_max(MaxHeap* maxHeap) {
    if (maxHeap->size <= 0) {
        return -1; 
    }

    if (maxHeap->size == 1) {
        maxHeap->size--;
        return maxHeap->arr[0]->index;
    }

    NodeMax* root_node = maxHeap->arr[0];

    maxHeap->arr[0] = maxHeap->arr[maxHeap->size - 1];
    maxHeap->size--;

    max_heapify(maxHeap, 0);

    return root_node->index;
}

void free_max_heap(MaxHeap* maxHeap) {
    if (maxHeap == NULL) {
        return;
    }
    free(maxHeap->arr);
    free(maxHeap);
}
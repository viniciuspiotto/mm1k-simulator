#include "min_heap.h"

void swap(Node **a, Node **b) {
    Node *temp = *a;
    *a = *b;
    *b = temp;
}

MinHeap* create_min_heap(int capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->capacity = capacity;
    minHeap->size = 0;
    minHeap->arr = (Node**)malloc(capacity * sizeof(Node*));
    return minHeap;
}

int parent(int i) {
    return (i - 1) / 2;
}

int left_child(int i) {
    return (2 * i + 1);
}

int right_child(int i) {
    return (2 * i + 2);
}

void min_heapify(MinHeap* minHeap, int i) {
    int l = left_child(i);
    int r = right_child(i);
    int smallest = i;

    if (l < minHeap->size && minHeap->arr[l]->arrived_time < minHeap->arr[smallest]->arrived_time) {
        smallest = l;
    }

    if (r < minHeap->size && minHeap->arr[r]->arrived_time < minHeap->arr[smallest]->arrived_time) {
        smallest = r;
    }

    if (smallest != i) {
        swap(&minHeap->arr[i], &minHeap->arr[smallest]);
        min_heapify(minHeap, smallest);
    }
}

void insert_node(MinHeap* minHeap, Node* node) {
    minHeap->size++;
    int i = minHeap->size - 1;
    minHeap->arr[i] = node;

    while (i != 0 && minHeap->arr[parent(i)]->arrived_time > minHeap->arr[i]->arrived_time) {
        swap(&minHeap->arr[i], &minHeap->arr[parent(i)]);
        i = parent(i);
    }
}

int extract_min(MinHeap* minHeap) {
    if (minHeap->size == 1) {
        minHeap->size--;
        return minHeap->arr[0]->index;
    }

    Node* root_node = minHeap->arr[0];

    minHeap->arr[0] = minHeap->arr[minHeap->size - 1];
    minHeap->size--;

    min_heapify(minHeap, 0);

    return root_node->index;
}
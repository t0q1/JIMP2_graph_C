#ifndef LOAD_GRAPH_H
#define LOAD_GRAPH_H

#define BUFSIZE 8192
#define MAXLINES 100000

typedef struct {
    int *data;      // wskaźnik na dane
    int size;       // liczba elementów
    int capacity;   // pojemność tablicy
} DynamicArray;

void initArray(DynamicArray *arr, int initialCapacity);

void append(DynamicArray *arr, int value);

void freeArray(DynamicArray *arr);

void printArray(DynamicArray *arr);

Graph *load_graph(const char *filename);

#endif //LOAD_GRAPH_H

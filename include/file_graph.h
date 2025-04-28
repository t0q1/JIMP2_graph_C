#ifndef FILE_GRAPH_H
#define FILE_GRAPH_H

#include <stdbool.h>

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

void save_graph(Graph * g, const char *filename, bool terminal_output, bool file_output, bool binary, int valid_division);

#endif //FILE_GRAPH_H
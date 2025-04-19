#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/graph.h"
#include "../include/load_graph.h"

void initArray(DynamicArray *arr, int initialCapacity) {
    arr->data = malloc(initialCapacity * sizeof(int));
    arr->size = 0;
    arr->capacity = initialCapacity;
}

void append(DynamicArray *arr, int value) {
    if (arr->size >= arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * sizeof(int));
    }
    arr->data[arr->size++] = value;
}

void freeArray(DynamicArray *arr) {
    free(arr->data);
    arr->size = 0;
    arr->capacity = 0;
}

void printArray(DynamicArray *arr) {
    for (int i = 0; i < arr->size; i++)
        printf("%d ", arr->data[i]);
    printf("\n");
}

Graph *load_graph(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Blad: Nie udalo sie otworzyc pliku wejsciowego o podanej sciezkce (%s). Przerywam dzialanie.\n", filename);
        exit(EXIT_FAILURE);
    }

    char * line = NULL;
    size_t len = 0;
    size_t r;
    int line_num = 0;
    DynamicArray *arrays[5];
    for(int i = 0; i < 5; i++)
    {
        arrays[i] = malloc(sizeof(DynamicArray));
        initArray(arrays[i], 200);
    }


    // zczytanie kazdej linii
    while ((r=getline(&line, &len, f)) != -1 && line_num < 5) // getline dynamicznie alokuje bufor - nie trzeba sie przejmowac dlugoscia linii
    {
        if (line[r - 1] == '\n') line[r - 1] = '\0';
        
        char * token = strtok(line, ";"); // strtok dzieli tekst do najblizszego znaku ";"
        char * end;
        while(token != NULL)
        {
            int num = strtol(token, &end, 10);
            
            if (*end != '\0' || num < 0)
            {
                fprintf(stderr, "Dane w pliku przedstawiajace graf sa niepoprawne. Przerywam dzialanie.1");
                fclose(f);
                exit(EXIT_FAILURE);
            }
            append(arrays[line_num], num);
            token = strtok(NULL, ";");
        }
        
        line_num++;
    }
    fclose(f);

    for(int i =0; i< 5; i++)
    {
        printArray(arrays[i]);
    }

    int max_num = arrays[0]->data[0];

    // linijka 1 i 2 --indeksowane od 0 
    int size2 = arrays[2]->size;
    int size1 = arrays[1]->size;
    // tego jednak nie trzeba
    // if (arrays[2]->data[size2-1] != size1) // jesli liczba w lin1 i lin2 sie nie zgadza
    // {
    //     fprintf(stderr, "Dane w pliku przedstawiajace graf sa niepoprawne. Przerywam dzialanie.2");
    //     exit(EXIT_FAILURE);
    // }
    
    for ( int i = 0; i < size2 - 1; i++) // jesli sa malejace
    {
        if (arrays[2]->data[i] > arrays[2]->data[i + 1])
        {
            fprintf(stderr, "Dane w pliku przedstawiajace graf sa niepoprawne. Przerywam dzialanie.3");
            exit(EXIT_FAILURE);
        }
    }
    
    for (int i = 0; i < size1; i++) // jesli wychodzi poza macierz
    {
        if (arrays[1]->data[i] > max_num)
        {   
            // printf("%d\n", arrays[2]->data[i]);
            fprintf(stderr, "\ny4:%d, %d\n", arrays[1]->data[i], max_num);
            fprintf(stderr, "Dane w pliku przedstawiajace graf sa niepoprawne. Przerywam dzialanie.4");
            exit(EXIT_FAILURE);
        }
    }
    // printf("yolinta");
    Graph * graph = createGraph(size1);

    // linijka 3 i 4 -ind od 0
    
    
    int size4 = arrays[4]->size;
    int size3 = arrays[3]->size;

    // tego jednak nie trzeba
    // if (arrays[4]->data[size4-1] != size3) // jesli liczba w lin3 i lin4 sie nie zgadza
    // {
    //     fprintf(stderr, "\ny5:%d, %d\n", arrays[4]->data[size4-1], size3);
    //     fprintf(stderr, "Dane w pliku przedstawiajace graf sa niepoprawne. Przerywam dzialanie.5");
    //     exit(EXIT_FAILURE);
    // }

    for ( int i = 0; i < size4 - 1; i++) // jesli sa malejace
    {
        if (arrays[4]->data[i] > arrays[4]->data[i + 1])
        {
            fprintf(stderr, "Dane w pliku przedstawiajace graf sa niepoprawne. Przerywam dzialanie.6");
            exit(EXIT_FAILURE);
        }
    }
    
    int prev = 0;
    int nxt = 0;
    int first_node = -1;
    for(int i = 0; i <= size4; i++)
    {
        nxt = arrays[4]->data[i];
        if (i == size4) nxt = size3;

        for(int j = prev; j < nxt; j++)
        {
            if (j == prev) 
            {
                first_node = arrays[3]->data[j];
                continue;
            }
            addEdge(graph, first_node, arrays[3]->data[j]);
        }

        prev = arrays[4]->data[i];
    }
    return graph;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/graph.h"
#include "../include/load_graph.h"

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
    char * line2;
    char * line3;
    char * line4;
    char * line5;
    int max_num = 0;
    
    // zczytanie kazdej linii
    while ((r=getline(&line, &len, f)) != -1 && line_num < 5) // getline dynamicznie alokuje bufor - nie trzeba sie przejmowac dlugoscia linii
    {
        if (line[r - 1] == '\n') line[r - 1] = '\0';

        if (line_num == 0)
        {
            char * token = strtok(line, ";"); // strtok dzieli tekst do najblizszego znaku ";"
            char * end;
            int num = strtol(token, &end, 10);

            if (*end != '\0' || num < 0)
            {
                fprintf(stderr, "Dane w pliku przedstawiajace graf sa niepoprawne. Przerywam dzialanie.");
                return 0;
            }
        }
        if(line_num == 1)
        {
            line2 = malloc(r + 1);
            strcpy(line2, line);
        }
        if(line_num == 2)
        {
            line3 = malloc(r + 1);
            strcpy(line2, line);
        }
        if(line_num == 3)
        {
            line4 = malloc(r + 1);
            strcpy(line2, line);
        }
        if(line_num == 4)
        {
            line5 = malloc(r + 1);
            strcpy(line2, line);
        }
        line_num++;
    }
    fclose(f);

    // linijka 2 i 3
    char * token = strtok(line3, ";");
    char * token2 = strtok(line2, ";");
    char * end = NULL;
    char* end2 = NULL;
    int prev = 0;
    int num = 0;

    while (token != NULL) // nie jest odporne na powtarzanie cyfr!!!!
    {
        num = strtol(token, &end, 10);
        if (*end != '\0' || num < 0 | num < prev)
        {
            fprintf(stderr, "Dane w pliku przedstawiajace graf sa niepoprawne. Przerywam dzialanie.");
            exit(EXIT_FAILURE);
        }

        for ( int i = prev; i < num; i++)
        {
            int val = strtol(token2, &end2, 10);
            if (*end2 != '\0' || val < 0 | val > max_num)
            {
                fprintf(stderr, "Dane w pliku przedstawiajace graf sa niepoprawne. Przerywam dzialanie.");
                exit(EXIT_FAILURE);
            }
            token2 = strtok(NULL, ";");
        }

        prev = num;
        token = strtok(NULL, ";");
    }

    printf("%d\n", num);
    Graph * graph = createGraph(num);

    // linijka 4 i 5
    token = strtok(line5, ";");
    token2 = strtok(line4, ";");
    end = NULL;
    end2 = NULL;
    prev = 0;
    num = 0;

    while (token != NULL)
    {
        num = strtol(token, &end, 10);
        if (*end != '\0' || num < 0 | num < prev)
        {
            fprintf(stderr, "Dane w pliku przedstawiajace graf sa niepoprawne. Przerywam dzialanie.");
            exit(EXIT_FAILURE);
        }
        int start = 0;
        for ( int i = prev; i < num; i++)
        {
            int val = strtol(token2, &end2, 10);
            if (*end2 != '\0' || val < 0 | val > max_num)
            {
                fprintf(stderr, "Dane w pliku przedstawiajace graf sa niepoprawne. Przerywam dzialanie.");
                exit(EXIT_FAILURE);
            }
            if (i == prev) start = val;
            else addEdge(graph, start, val);
            token2 = strtok(NULL, ";");
        }

        prev = num;
        token = strtok(NULL, ";");
    }

    return graph;
}

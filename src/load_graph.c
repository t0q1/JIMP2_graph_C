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
    ssize_t r;
    int line_num = 0;

    char * line2 = NULL;
    char * line3 = NULL;
    char * line4 = NULL;

    while ((r=getline(&line, &len, f)) != -1 && line_num < 5) // getline dynamicznie alokuje bufor - nie trzeba sie przejmowac dlugoscia linii
    {
        if (line_num == 1 ) // linie 2 i 3 zapisuje - tak samo wygladaja na wyjsciu i nie sa do niczego potrzebne
        {
            line2 = malloc(r + 1);
            strcpy(line2, line);
            line_num++;
            continue;
        }
        if (line_num == 2)
        {
            line3 = malloc(r + 1);
            strcpy(line3, line);
            line_num++;
            continue;
        }
        if (line_num == 3) // linie 4 zapisuje, bo z zaleznosci od linii 5 jest ona "dzielona"
        {
            line4 = malloc(r + 1);
            strcpy(line4, line);
            line_num++;
            continue;
        }

        if (line[r - 1] == '\n') line[r - 1] = '\0';

        char * token = strtok(line, ";"); // strtok dzieli tekst do najblizszego znaku ";"
        while (token != NULL)
        {
            // int liczba = atoi(token);
            // if (line_num == 0) graph = createGraph(liczba);
            // else
            // {
            //     for(int i = 0; i < liczba; i++)
            //     {

            //     }
            // }
            // // printf(" %d ", liczba);
            // token = strtok(NULL, ";");
        }
        // printf("\n");
        line_num++;
    }

    printf("\n%s\n%s", line2, line3);
    fclose(f);

    return 0;
}

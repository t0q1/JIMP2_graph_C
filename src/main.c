#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../include/graf.h"

#define BUFSIZE 8192
#define MAXLINES 100000

int main(int argc, char ** argv)
{   
    // TestGraph();
    if (argc <= 1)
    {
        fprintf(stderr, "Zbyt mala liczba argumentow. Nalezy podac sciezke pliku wejsciowego!");
        return 1;
    }

    FILE *in = argc > 1 ? fopen(argv[1], "r") : NULL;

    if (in == NULL)
    {
        fprintf(stderr, "Nie udalo sie otworzyc pliku wejsciowego o podanej sciezkce. Przerywam dzialanie.");
        return 1;
    }

    int N = argc > 2 ? atoi(argv[2]) : 1;
    if (N < 1) 
    {
        fprintf(stderr, "Liczba podzielen grafu musi byc wieksza badz rowna 1. Przerywam dzialanie.");
        return 1;
    }

    int M = argc > 3 ? atoi(argv[3]) : 10;
    if (M < 0 || M > 100) 
    {
        fprintf(stderr, "Liczba marginesu roznicy procentowej miedzy wierzcholkami powstalych grafow musi znajdowac sie w przedziale [0-100]. Przerywam dzialanie.");
        return 1;
    }

    int opt;
    int binary_output = 0;
    int terminal_output = 0;
    int file_output = 0;
    char * output_filename = "wynik.txt";
    while ((opt = getopt(argc, argv, "bto:")) != -1)
    {
        switch (opt) {
            case 'b':
                binary_output = 1;
                break;
            case 't':
                terminal_output = 1;
                break;
            case 'o':
                file_output = 1;
                char * output_filename = optarg;
                break;
        }
    }
    if (terminal_output == 0) file_output = 1; // jesli flaga t bez o, to tylko terminal. Jesli brak jakiejkolwiek flagi to wyjscie to pliku


    // wczytywanie z pliku, narazie bez zabezpieczen
    Graph * graph = NULL;

    char * line = NULL;
    size_t len = 0;
    ssize_t r;
    int line_num = 0;

    char * line2 = NULL;
    char * line3 = NULL;
    char * line4 = NULL;

    while ((r=getline(&line, &len, in)) != -1 && line_num < 5) // getline dynamicznie alokuje bufor - nie trzeba sie przejmowac dlugoscia linii
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
    fclose(in);
    return 0;
}
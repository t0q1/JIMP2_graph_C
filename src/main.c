#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "../include/getopt.h"
#include "../include/graph.h"
#include "../include/file_graph.h"

#ifdef _WIN32
#define SYS 0
#else
#define SYS 1
#endif


const char *help_menu = "------POMOC------\n"
                            "Argumenty:\n"
                            " (1)\t<path>\t- plik wejsciowy z grafem\n"
                            " (2)\t<N>\t- liczba podzielen grafu (opcjonalny)\n"
                            " (3)\t<M>\t- liczba marginesu roznicy procentowej miedzy wierzcholkami powstalych grafow (opcjonalny)\n"
                            "Flagi:\n"
                            "\t-h\t- wyswietla pomoc\n"
                            "\t-b\t- wynik wyswietlany w formie binarnej\n"
                            "\t-t\t- wynik operacji wyswietlany w terminalu\n"
                            "\t-o <path>\t- wynik operacji zapisywany w podanym pliku\n";

bool binary_output = false;
bool terminal_output = false;
bool file_output = false;
char *output_filename = "wynik.txt";

int safe_atoi(const char *text) {
    char *endptr;
    long value = strtol(text, &endptr, 10);

    if (value < INT_MIN || value > INT_MAX) {
        fprintf(stderr, "Blad: Liczba podzieleń grafu została niepoprawnie zdefiniowana. Przerywam działanie.\n");
        exit(EXIT_FAILURE);
    }

    if (*endptr != '\0') {
        fprintf(stderr, "Blad: Liczba podzieleń grafu została niepoprawnie zdefiniowana. Przerywam działanie.\n");
        exit(EXIT_FAILURE);
    }

    return (int)value;
}

int main(int argc, char **argv) {
    char opt;
    while ((opt = ogetopt(argc, argv, "hbto:")) != -1) {
        switch (opt) {
            case 'h':
                printf("%s\n", help_menu);
                break;
            case 'b':
                binary_output = true;
                break;
            case 't':
                terminal_output = true;
                break;
            case 'o':
                file_output = true;
                output_filename = fargv[0];
                break;
            case '?':
                fprintf(stderr, "Blad: Nieznana flaga -%c. Przerywam dzialanie.\n", cflag);
                exit(EXIT_FAILURE);
            case ':':
                fprintf(stderr, "Blad: Flaga -%c wymaga argumentu. Przerywam dzialanie.\n", cflag);
                exit(EXIT_FAILURE);
        }
    }
    if (vargc <= SYS) {
        fprintf(stderr, "Blad: Zbyt mala liczba argumentow. Nalezy podac sciezke pliku wejsciowego. Przerywam dzialanie.\n");
        return EXIT_FAILURE;
    }

    int n = vargc > SYS+1 ? safe_atoi(vargv[SYS+1]) : 1;
    if (n < 1) {
        fprintf(stderr, "Blad: Liczba podzielen grafu musi byc wieksza badz rowna 1. Przerywam dzialanie.\n");
        return EXIT_FAILURE;
    }

    int m = vargc > SYS+2 ? safe_atoi(vargv[SYS+2]) : 10;
    if (m < 0 || m > 100) {
        fprintf(stderr, "Blad: Liczba marginesu roznicy procentowej miedzy wierzcholkami powstalych grafow musi znajdowac sie w przedziale [0-100]. Przerywam dzialanie.\n");
        return EXIT_FAILURE;
    }

    // jesli jest flaga -t bez flagi -o, to wyświetlanie tylko w terminalu.
    // Jesli brak jakiejkolwiek flagi to wyjscie do pliku
    if (terminal_output == false)
        file_output = true;

    // wczytywanie grafu z pliku
    Graph *graph = load_graph(vargv[SYS]);
    ListOfGraphs *result = init_list_of_graphs(n + 1);

    int division = recursive_partition(&graph, n, m, result);

    save_graph(graph, output_filename, terminal_output, file_output, binary_output, division);
    freeGraph(graph);
    freeListOfGraphs(&result);
    return EXIT_SUCCESS;
}

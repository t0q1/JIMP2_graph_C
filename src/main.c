#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "../include/graph.h"
#include "../include/file_graph.h"

int main(int argc, char ** argv)
{   
    // TestGraph();
    if (argc <= 1)
    {
        fprintf(stderr, "Zbyt mala liczba argumentow. Nalezy podac sciezke pliku wejsciowego!");
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

    // wczytywanie grafu z pliku
    Graph *graph = load_graph(argv[1]);
    printGraph(graph);
    
    // ListOfGraphs * result = malloc(sizeof(ListOfGraphs));
    // for (int i = 0; i < N + 1; i++)
    // {
    //     result->subgraphs[i] = NULL;
    // }
    // printf("\nyolo\n");
    // recursive_partition(graph, N, M, result);
    
    // printf("\nyolo\n");

    // for (int i = 0; i < N + 1; i++)
    // {
    //     if (result->subgraphs[i] != NULL) printGraph(result->subgraphs[i]);

    //     printf("\n-\n");
    // }

    save_graph(graph, output_filename, terminal_output, binary_output);
    
    return EXIT_SUCCESS;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../include/graph.h"
#include "../include/file_graph.h"

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


DynamicArray *arrays[5];
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
                fprintf(stderr, "Blad: Dane w pliku przedstawiajace graf sa niepoprawne. Przerywam dzialanie.\n");
                fclose(f);
                exit(EXIT_FAILURE);
            }
            append(arrays[line_num], num);
            token = strtok(NULL, ";");
        }
        
        line_num++;
    }
    fclose(f); // ZAMYKAM PLIK!!!

    int max_num = arrays[0]->data[0];

    // linijka 1 i 2 --indeksowane od 0 
    int size2 = arrays[2]->size;
    int size1 = arrays[1]->size;

    for ( int i = 0; i < size2 - 1; i++) // jesli sa malejace
    {
        if (arrays[2]->data[i] > arrays[2]->data[i + 1])
        {
            fprintf(stderr, "Blad: Dane w pliku przedstawiajace graf sa niepoprawne. Przerywam dzialanie.\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < size2 - 1; i++)
    {
        if (arrays[2]->data[i + 1] - arrays[2]->data[i + 1] > max_num)
        {
            fprintf(stderr, "Blad: Dane w pliku przedstawiajace graf sa niepoprawne. Przerywam dzialanie.\n");
            exit(EXIT_FAILURE);
        }
    }
    if (arrays[2]->data[size2 - 1] < size1 && size1 - arrays[2]->data[size2 - 1] > max_num)
    {
        fprintf(stderr, "Blad: Dane w pliku przedstawiajace graf sa niepoprawne. Przerywam dzialanie.\n");
        exit(EXIT_FAILURE);
    }
    Graph * graph = createGraph(size1);

    // linijka 3 i 4 -ind od 0
    int size4 = arrays[4]->size;
    int size3 = arrays[3]->size;

    for ( int i = 0; i < size4 - 1; i++) // jesli sa malejace
    {
        if (arrays[4]->data[i] > arrays[4]->data[i + 1])
        {
            fprintf(stderr, "Blad: Dane w pliku przedstawiajace graf sa niepoprawne. Przerywam dzialanie.\n");
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

void reverse(char *bin, int left, int right) { // odwraca tablice

    while (left < right) {
        char temp = bin[left];
        bin[left] = bin[right];
        bin[right] = temp;
        left++;
        right--;
    }
}

char* decToBinary(int n) { // przeksztalcanie liczby calkowitej na postac binarna

	char* bin = malloc(33 * sizeof(char));

    for (int i = 0; i < 32; i++)
        bin[i] = '0';
    bin[32] = '\0';

    if (n == 0) return bin;

    int index = 0;
    while (n > 0) {
        int bit = n % 2;
        bin[index++] = '0' + bit;
        n /= 2;
    }

	reverse(bin, 0, 31);
  	return bin;
}

void print(bool terminal, bool file_output, bool binary, FILE *f, const char *style, int argument) {
    if (binary) {
        char * b_semicolon = "11111111111111111111111111111110";
        char * b_newline = "11111111111111111111111111111111";

        char * new_style = calloc(100, sizeof(char));
        char * head_new_style = new_style;

        while (*style) { // przeksztalcenie formatu na ten zgodny z binarnym
            if (*style == ';') {

                for (int i = 0; i < 32; i++) {
                    *new_style = b_semicolon[i];
                    new_style++;
                }

            }
            else if (*style == '\n') {

                for (int i = 0; i < 32; i++) {
                    *new_style = b_newline[i];
                    new_style++;
                }

            }
            else if (*style == 'd') {
                *new_style = 's';
                new_style++;
            }
            else {
                *new_style = *style;
                new_style++;
            }

            style++;
        }

        char * new_val = decToBinary(argument); // przeksztalcenie argumentu

        if (terminal) // wypisuje gdy terminal
            argument != -1 ? printf(head_new_style, new_val) : printf(head_new_style); // w zaleznosci od argumentu -> odpowiendie wypisanie

        if (file_output) // wypisuje gdy plik
            argument != -1 ? fprintf(f, head_new_style, new_val) : fprintf(f, head_new_style);
        free(head_new_style);

    } else {
        if (terminal) // wypisuje gdy terminal
            argument != -1 ? printf(style, argument) : printf(style);

        if (file_output) // wypisuje gdy plik
            argument != -1 ? fprintf(f, style, argument) : fprintf(f, style);
    }
}


void save_graph(Graph * g, const char *filename, bool terminal_output, bool file_output, bool binary, int valid_division)
{
    //wczytanie pliku
    FILE *out = fopen(filename, "w");

    if (out == NULL)
    {
        fprintf(stderr, "Blad: Nie udalo sie otworzyc pliku wyjsciowego o podanej sciezkce (%s). Przerywam dzialanie.\n", filename);
        exit(EXIT_FAILURE);
    }

    if (!binary)
      print(terminal_output, file_output, binary, out, "%d\n", valid_division);

    // wypiswanie pierwszych 3 linijek
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < arrays[i]->size; j++)
        {
            int val = arrays[i]->data[j];

            print(terminal_output, file_output, binary, out, "%d", val);

            if (j != arrays[i]->size - 1)
                print(terminal_output, file_output, binary, out, ";", -1);
            else
                print(terminal_output, file_output, binary, out, "\n", -1);
        }
    }

    DynamicArray * lengths = malloc(sizeof(DynamicArray)); // tablica do przechowania liczbe sasiadow kolejnych wezlow (potrzebna do linijki 5)
    initArray(lengths, 200);
    int last_index = 0;

    // wypisanie linijki 4
    for (int i = 0; i < g->n; i++)
    {
        if (g->adj[i] != NULL)
        {
            if (i == 0) print(terminal_output, file_output, binary, out, "%d", i);
            else  print(terminal_output, file_output, binary, out, ";%d", i);

            Node * node = g->adj[i];
            int counter = 0;
            while (node)
            {
                counter++;
                print(terminal_output, file_output, binary, out, ";%d", node->vertex);
                node = node->next;
            }
            append(lengths, counter);
            last_index = i;
        }
        else append(lengths, 0);
    }

    //wypisanie linijki 5

    int prev = 0;
    print(terminal_output, file_output, binary, out, "\n%d", prev);

    for (int i = 0; i < last_index; i++)
    {
        if (lengths->data[i])
        {
            prev += lengths->data[i] + 1;
            print(terminal_output, file_output, binary, out, ";%d", prev);
        }

    }
    freeArray(lengths);
    fclose(out);
}
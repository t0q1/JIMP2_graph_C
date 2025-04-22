#include "../include/graph.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

Node *createNode(int vertex) { // tworzy wierzcholek
    Node * newNode = malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

Graph *createGraph(int n) { // tworzy graf
    Graph * graph = malloc(sizeof(Graph));
    graph->n = n;

    graph->adj = malloc(n * sizeof(Node *));

    for (int i = 0; i < n; i++)
        graph->adj[i] = NULL;

    return graph;
}

void addEdge(Graph* graph, int u, int v) { // laczy dwa wierzcholki
    Node* newNode = createNode(v);
    newNode->next = graph->adj[u];
    graph->adj[u] = newNode;

    newNode = createNode(u);
    newNode->next = graph->adj[v];
    graph->adj[v] = newNode;
}

void printGraph(Graph *graph) { // wypisuje graf
    for (int i = 0; i < graph->n; i++)
    {
        Node * temp = graph->adj[i];
        printf("\nWierzcholek %d: ", i);
        while (temp)
        {
            printf("%d -> ", temp->vertex);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

void freeGraph(Graph * graph) { // zwalnia pamiec grafu
    for (int i = 0; i < graph->n; i++) {
        Node * temp = graph->adj[i];
        while (temp) {
            Node * toDelete = temp;
            temp = temp->next;
            free(toDelete);
        }
    }
    free(graph->adj);
    free(graph);
}

int * partition(Graph * g) // dokonuje podzialu grafu na 2 czesci zachowujac minimalna liczbe przeciec
{
    int n = g->n;
    int *part = malloc(n * sizeof(int));
    int countA = 0, countB = 0;

    for (int i = 0; i < n; i++)
    {
        if (countA < n / 2)
        {
            part[i] = 0;
            countA++;
        } 
        else
        {
            part[i] = 1;
            countB++;
        }
    }

    bool improved = true;
    while (improved)
    {
        improved = false;
        for (int u = 0; u < n; u++)
        {
            int currentPart = part[u];
            int otherPart = 1 - currentPart;
            int external = 0, internal = 0;
            Node * neighbor = g->adj[u];
            while (neighbor)
            {
                if (part[neighbor->vertex] == currentPart)
                    internal++;
                else
                    external++;
                neighbor = neighbor->next;
            }

            int gain = external - internal;
            if (gain > 0)
            {
                if ((currentPart == 0 && countA > countB) || (currentPart == 1 && countB > countA))
                {
                    part[u] = otherPart;
                    if (currentPart == 0)
                    {
                        countA--;
                        countB++;
                    }
                    else
                    {
                        countB--;
                        countA++;
                    }
                    improved = true;
                }
            }
        }
    }
    return part;

}

bool margin_ok(int sizeA, int sizeB, int margin_percent) { // sprawdza czy utworzone grafy mieszcza sie w marginesie
    int total = sizeA + sizeB;
    int diff = (int)(abs(sizeA - sizeB) / (double)total * 200);
    return diff <= margin_percent;
}

void add_partition(ListOfGraphs *list, Graph *g) { // dodaje partycje do listy podgrafow
    if (list->count < 1000) { // 1000 bo taka jest max dlugosc tablicy
        list->subgraphs[list->count++] = g;
    }
}


int find_largest_partition(ListOfGraphs *list) { // zwraca index najwieszkego podgrafu
    int max_index = -1;
    int max_size = -1;
    for (int i = 0; i < list->count; i++) {
        int size = list->subgraphs[i]->n; // liczba wierzcholkow
        if (size > max_size) {
            max_size = size;
            max_index = i;
        }
    }
    return max_index;
}

Graph * extract_subgraph(Graph *g, int * part, int n, int partition_value) /* tworzy podgraf - niestety tym sposobem kazdy 
podgraf ma tablice wierzcholkow rowna n, czyli tyle ile graf glowny*/
{
    // Graph *subgraph = createGraph(n);
    // for (int i = 0; i < n; i++)
    // {
    //     if (part[i] == partition_value)
    //     {
    //         Node * neighbor = g->adj[i];
    //         while (neighbor != NULL)
    //         {
    //             int v = neighbor->vertex;
    //             if (part[v] == partition_value) addEdge(subgraph, i, v);
    //         }
    //         neighbor = neighbor->next;
    //     }
    // }
    // return subgraph;
}

int count(int* array, int n, int value) // zlicza liczbe elementow o podanej wartosci w tablicy
{
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (array[i] == value) {
            count++;
        }
    }
    return count;
}

void recursive_partition(Graph *g, int k, double margin_percent, ListOfGraphs *result) {
    add_partition(result, g);

    while (result->count < k) {
        int index = find_largest_partition(result);
        if (index == -1) break;

        Graph *to_split = result->subgraphs[index];
        int n = to_split->n;
        int *part = partition(to_split); 

        int sizeA = count(part, n, 0);
        int sizeB = count(part, n, 1);

        if (!margin_ok(sizeA, sizeB, margin_percent)) break; // nie udalo sie podzielic

        Graph *A = extract_subgraph(to_split, part, n, 0);
        Graph *B = extract_subgraph(to_split, part, n, 1);

        // Podmieniamy stara partycje dwiema nowymi
        result->subgraphs[index] = A;
        add_partition(result, B);

        free(part);
    }
} 

void TestGraph()
{
    Graph* graph = createGraph(5);
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 4);
    addEdge(graph, 1, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 1, 4);
    addEdge(graph, 2, 3);
    addEdge(graph, 3, 4);

    printGraph(graph);
}
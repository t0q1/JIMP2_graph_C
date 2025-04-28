#include "../include/graph.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

Node *createNode(int vertex) { // tworzy wierzcholek
    Node * newNode = malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

Graph *createGraph(int n) { // tworzy graf
    Graph * graph = malloc(sizeof(Graph));
    graph->n = n;
    graph->start = 0;
    graph->parent = 0;

    graph->adj = malloc(n * sizeof(Node *));

    for (int i = 0; i < n; i++)
        graph->adj[i] = NULL;

    return graph;
}

void addEdge(Graph* graph, int u, int v) { // laczy dwa wierzcholki
    Node* newNode = createNode(v);
    newNode->next = graph->adj[u];
    graph->adj[u] = newNode;
}

void printGraph(Graph *graph) { // wypisuje graf
    for (int i = 0; i < graph->n; i++)
    {
        Node * temp = graph->adj[i];
        printf("Wierzcholek %d: ", i + graph->parent);
        while (temp)
        {
            printf("%d -> ", temp->vertex + graph->parent);
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

int get_min_index(int *part, int n, int partition_value) {
    int min = n;
    for (int i = n - 1; i >= 0; i--)
        if (part[i] == partition_value)
            min = i;
    return min;
}

int get_max_index(int *part, int n, int partition_value) {
    int max = 0;
    for (int i = 0; i < n; i++)
        if (part[i] == partition_value)
            max = i;
    return max;
}

Graph *extract_subgraph(Graph *g, int *part, int n, int partition_value) { // tworzy podgraf
    int difference = get_max_index(part, n, partition_value) - get_min_index(part, n, partition_value);

    Graph *subgraph = createGraph(difference + 1);

    for (int i = 0; i < n; i++) {
        if (part[i] == partition_value) {
            Node *neighbor = g->adj[i];
            while (neighbor) {
                int v = neighbor->vertex;
                if (part[v] == partition_value)
                    addEdge(subgraph, i - g->start, v - g->start);
                neighbor = neighbor->next;
            }
        }
    }
    subgraph->parent = !partition_value ? g->parent : g->parent + difference;
    g->start += difference + 1;
    return subgraph;
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

int recursive_partition(Graph **g, int k, double margin_percent, ListOfGraphs *result) {
    int counter = 0;

    add_partition(result, *g);
    while (result->count < k + 1) {
        int index = find_largest_partition(result);
        if (index == -1) break;

        Graph *to_split = result->subgraphs[index];
        int n = to_split->n;
        int *part = partition(to_split);

        int sizeA = count(part, n, 0);
        int sizeB = count(part, n, 1);

        if (!margin_ok(sizeA, sizeB, margin_percent)) 
        {
            fprintf(stderr, "Blad: Nie udalo sie podzielic grafu\n");
            break;
        }

        Graph *A = extract_subgraph(to_split, part, n, 0);
        Graph *B = extract_subgraph(to_split, part, n, 1);

        // Podmieniamy stara partycje dwiema nowymi
        result->subgraphs[index] = A;
        add_partition(result, B);

        counter++;
        free(part);
    }

    // Scalanie podgrafów w jeden
    Graph *newGraph = createGraph((*g)->n);
    int i = 0;
    while (result->subgraphs[i]) {
        Graph *temp = result->subgraphs[i];
        for (int j = 0; j < temp->n; j++) {
            Node *tempNode = temp->adj[j];
            while (tempNode) {
                addEdge(newGraph, j + temp->parent, tempNode->vertex + temp->parent);
                tempNode = tempNode->next;
            }
        }
        i++;
    }
    *g = newGraph;

    return counter;
} 

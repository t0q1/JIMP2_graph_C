#include "../include/graph.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

Node *createNode(int vertex) {
    Node * newNode = malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

Graph *createGraph(int n) {
    Graph * graph = malloc(sizeof(Graph));
    graph->n = n;

    graph->adj = malloc(n * sizeof(Node *));

    for (int i = 0; i < n; i++)
        graph->adj[i] = NULL;

    return graph;
}

void addEdge(Graph* graph, int u, int v) {
    Node* newNode = createNode(v);
    newNode->next = graph->adj[u];
    graph->adj[u] = newNode;

    newNode = createNode(u);
    newNode->next = graph->adj[v];
    graph->adj[v] = newNode;
}

void printGraph(Graph *graph) {
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

void freeGraph(Graph * graph) {
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

int * partition(Graph * g)
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

// void recursive_partition(Graph* graph, int* part, int current_part, int num_parts, int margin_percent) {
//     if (num_parts == 1) return;

//     // Podział current_part na 2
//     int* sub_part = partition_2way(graph, margin_percent); // Zwraca tablicę 0/1

//     // Aktualizuj globalną tablicę `part` na np. 2*current_part i 2*current_part+1
//     for (int i = 0; i < graph->num_vertices; i++) {
//         if (part[i] == current_part) {
//             part[i] = (sub_part[i] == 0) ? (2 * current_part) : (2 * current_part + 1);
//         }
//     }

//     // Rekurencyjnie wywołaj na dwóch podgrafach
//     // (trzeba wyciągnąć te podgrafy z oryginalnego grafu!)
//     Graph* g0 = extract_subgraph(graph, part, 2 * current_part);
//     Graph* g1 = extract_subgraph(graph, part, 2 * current_part + 1);

//     recursive_partition(g0, part, 2 * current_part, num_parts / 2, margin_percent);
//     recursive_partition(g1, part, 2 * current_part + 1, num_parts / 2, margin_percent);

//     // zwolnij g0, g1
// }

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
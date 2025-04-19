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

// bool margin_ok(int sizeA, int sizeB, int margin_percent) {
//     int total = sizeA + sizeB;
//     int diff = (int)(abs(sizeA - sizeB) / (double)total * 200);
//     return diff <= margin_percent;
// }

// void add_partition(ListOfGraphs *list, Graph *g) {
//     if (list->count < 500) {
//         list->subgraphs[list->count++] = g;
//     }
// }

// int find_largest_partition(ListOfGraphs *list) {
//     int max_index = -1;
//     int max_size = -1;
//     for (int i = 0; i < list->count; i++) {
//         int size = count_vertices(list->subgraphs[i]);
//         if (size > max_size) {
//             max_size = size;
//             max_index = i;
//         }
//     }
//     return max_index;
// }

// void recursive_partition(Graph *g, int k, double margin_percent, ListOfGraphs *result) {
//     add_partition(result, g);

//     while (result->count < k) {
//         int index = find_largest_partition(result);
//         if (index == -1) break;

//         Graph *to_split = result->subgraphs[index];
//         int n = to_split->n;
//         int *part = partition(to_split); 

//         Graph *A = extract_subgraph(to_split, part, 0);
//         Graph *B = extract_subgraph(to_split, part, 1);

//         int sizeA = count_vertices(A);
//         int sizeB = count_vertices(B);

//         if (!margin_ok(sizeA, sizeB, margin_percent)) {
//             free(A);
//             free(B);
//             break; // nie udalo sie podzielic, konczymy
//         }

//         // Podmieniamy stara partycje dwiema nowymi
//         result->subgraphs[index] = A;
//         add_partition(result, B);

//         free(part);
//     }
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
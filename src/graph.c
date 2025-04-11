#include "../include/graph.h"

#include <stdio.h>
#include <stdlib.h>

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
#include "../include/graf.h"

#include <stdio.h>

Node * createNode(int vertex)
{
    Node * newNode = malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

Graph * createGraph(int n)
{
    Graph * graph = malloc(sizeof(Graph));
    graph->n = n;

    graph->adj = malloc(n * sizeof(Node *));

    for (int i = 0; i < n; i++)
    {
        graph->adj[i] = NULL;
    }

    return graph;
}

void addEdge(Graph* graph, int u, int v)
{
    Node* newNode = createNode(v);
    newNode->next = graph->adj[u];
    graph->adj[u] = newNode;

    newNode = createNode(u);
    newNode->next = graph->adj[v];
    graph->adj[v] = newNode;
}
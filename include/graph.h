#ifndef _GRAF_H_
#define _GRAF_H_

typedef struct n{
    int vertex;
    struct n* next;
} Node;

typedef struct {
    int n;
    Node ** adj;
} Graph;

typedef struct {
    Graph *subgraphs[1000];
    int count;
} ListOfGraphs;


Node * createNode(int vertex);
Graph * createGraph(int n);
void addEdge(Graph* graph, int u, int v);
void printGraph(Graph * graph);
void freeGraph(Graph * graph);
void TestGraph();
int * partition(Graph * g);
void recursive_partition(Graph *g, int k, double margin_percent, ListOfGraphs *result);
#endif
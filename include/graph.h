#ifndef _GRAF_H_
#define _GRAF_H_

typedef struct n{
    int vertex;
    struct n *next;
} Node;

typedef struct {
    int n;
    int start;
    int parent;
    Node **adj;
} Graph;

typedef struct {
    int n;
    Graph **subgraphs;
    int count;
} ListOfGraphs;

ListOfGraphs *init_list_of_graphs(int n);
void freeListOfGraphs(ListOfGraphs **list);

void addEdge(Graph* graph, int u, int v);
Graph *createGraph(int n);
void printGraph(Graph *graph);
void freeGraph(Graph *graph);
int recursive_partition(Graph **g, int k, double margin_percent, ListOfGraphs *result);
#endif
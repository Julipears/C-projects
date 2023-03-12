#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 20 //max length for a vertex name

typedef struct Enode{       //edge node struct
    char vertex[MAX_LEN];   //destination vertex name
    int weight;             //edge weight
    struct Enode *next;     //next edge node
} Enode;

typedef struct Vnode{       //vertex node struct
    char station[MAX_LEN];  //station name
    Enode *edges;           //pointer to first edge
    int cost;               //optional for you to use
    int visited;            //optional for you to use
    struct Vnode *prev;     //optional for you to use
} Vnode;

typedef struct Graph{   //graph struct
    Vnode **adj_list;   //pointer to array of vertex pointers
    int count;          //number of vertices in the graph
} Graph;

//lab5.c
char **plan_route(Graph *gr, char *start, char *dest);
void add(Graph *gr, char *station);
void 
update(Graph *gr, char *start, char *dest, int weight);
void disrupt(Graph *gr, char *station);

//extras.c
Graph *initialize_graph();
void print_gr(Graph* gr);
void free_gr(Graph* gr);
void print_route(char **route, char *start);

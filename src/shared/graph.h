#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
typedef struct {
    char name[256];
    int u;         
    int v;         
    double weight; 
} Edge;

typedef struct {
    int id;
    double x, y;
    int degree;    
    bool is_fixed;
} Node;

typedef struct {
    Node* nodes;
    int num_nodes;
    
    Edge* edges;   
    int num_edges;
} Graph;

#endif
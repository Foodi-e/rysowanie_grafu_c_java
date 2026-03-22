#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

typedef struct Edge {
    int target_id;      
    double weight;      
    struct Edge* next;  
} Edge;

typedef struct {
    int id;             
    double x, y;         
    Edge* neighbors;    
    int degree;
    bool is_fixed;         
} Vertex;

typedef struct {
    Vertex* vertices;
    int num_vertices;
} Graph;

#endif
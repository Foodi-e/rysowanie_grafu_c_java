#ifndef LOADER_H
#define LOADER_H

#include <stdio.h>
#include "../graph.h"

void print_edge_array(Graph * g);//do wywalenia?
Graph * load_from_file(FILE * fptr);
void add_degree(Graph * g, int * max_size_n);

#endif
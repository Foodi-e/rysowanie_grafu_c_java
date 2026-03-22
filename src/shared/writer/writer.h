#ifndef WRITER_H
#define WRITER_H

#include "../graph.h"

void write_to_txt(Graph* graph, const char* filename);
void write_to_bin(Graph* graph, const char* filename);

#endif
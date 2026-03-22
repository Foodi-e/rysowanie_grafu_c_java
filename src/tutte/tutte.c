#include "tutte.h"
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static int find_highest_degree_unfixed(Graph* graph) {
    int max_deg = -1;
    int best_idx = -1;
    for (int i = 0; i < graph->num_vertices; i++) {
        if (!graph->vertices[i].is_fixed && graph->vertices[i].degree > max_deg) {
            max_deg = graph->vertices[i].degree;
            best_idx = i;
        }
    }
    return best_idx;
}
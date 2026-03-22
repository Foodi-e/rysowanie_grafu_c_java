#include "tutte.h"
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static int find_highest_degree_unfixed(Graph* graph);
static void setup_boundary(Graph* graph, Config* config);
static void solve_linear_system(double* M, double* b, double* result, int n);

void run_tutte_embedding(Graph* graph, Config* config) {
    for (int i = 0; i < graph->num_vertices; i++) {
        graph->vertices[i].is_fixed = false;
    }
    setup_boundary(graph, config);
}


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

static void setup_boundary(Graph* graph, Config* config) {
    int k = config->k_fixed;

    if (k > graph->num_vertices) {
        k = graph->num_vertices; 
    }

    int* boundary_vertices = malloc(k * sizeof(int));

    if (config->selection_mode == CUSTOM) {
        for (int i = 0; i < k; i++) {
            boundary_vertices[i] = i;
            graph->vertices[i].is_fixed = true;
        }
    } else {
        for (int i = 0; i < k; i++) {
            int best_idx = find_highest_degree_unfixed(graph);
            if (best_idx != -1) {
                boundary_vertices[i] = best_idx;
                graph->vertices[best_idx].is_fixed = true;
            }
        }
    }

    //rozmieszczanie wierzchołkow na okręgu
    for (int i = 0; i < k; i++) {
        int idx = boundary_vertices[i];
        double angle = 2.0 * M_PI * i / k;
        graph->vertices[idx].x = config->radius * cos(angle);
        graph->vertices[idx].y = config->radius * sin(angle);
    }

    free(boundary_vertices);
}
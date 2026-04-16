#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/shared/data_loader/data_loader.h"
#include "../src/shared/graph.h"
#include "../src/shared/config.h"
#include "../src/tutte/tutte.h"
#include "../src/shared/writer/writer.h"

int main(int argc, char *argv[]){
    printf("\n--------------------------data_loader--------------------------\n");
    FILE *f = fopen("data/test_loader_missing_id.txt", "r");
    if(!f) {
        printf("couldnt read file\n");
        return EXIT_FAILURE;
    }
    Graph *g = load_from_file(f);
    for(int i = 0; i < g->num_nodes; i++){
        printf("id, deg: %d, %d\n",  g->nodes[i].id, g->nodes[i].degree);
    }
    for(int i = 0; i < g->num_edges; i++){
        int l = strlen(g->edges[i].name);
        for(int j = 0; j < l; j++){
            printf("%c", g->edges[i].name[j]);
        }
        printf(" %d %d %f\n", g->nodes[g->edges[i].u].id, g->nodes[g->edges[i].v].id, g->edges[i].weight);
    }
    fclose(f);
    printf("\n---------------------------------------------------------\n");

    printf("\n--------------------------tutte--------------------------\n");
    Config config;
    config.algorithm = TUTTE;
    config.k_fixed = 4;
    config.radius = 10;
    config.selection_mode = CUSTOM;
    //oczekiwane wartości: koło z punktem w środku
    f = fopen("data/test_tutte_circle.txt", "r");
    if(!f) {
        printf("couldnt read file\n");
        return EXIT_FAILURE;
    }
    g = load_from_file(f);
    run_tutte_embedding(g, &config); 
    write_to_txt(g, "output.txt");

    
    printf("\n---------------------------------------------------------\n");

    return 0;
}
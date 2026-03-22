#include <stdio.h>
#include <stdlib.h>
#include "../graph.h"

// zapis do zwykłego pliku tekstowego (.txt)
void write_to_txt(Graph* graph, const char* filename) {
    FILE* out = fopen(filename, "w");
    if (!out) {
        printf("Błąd: Nie można otworzyć pliku TXT do zapisu: %s\n", filename);
        return;
    }

    // nagłowek
    fprintf(out, "ID X Y\n");

    for (int i = 0; i < graph->num_nodes; i++) {
        // Zapisujemy oryginalne ID z pliku oraz obliczone współrzędne
        fprintf(out, "%d %f %f\n", graph->nodes[i].id, graph->nodes[i].x, graph->nodes[i].y);
    }

    fclose(out);
    printf("Pomyślnie zapisano współrzędne do pliku tekstowego: %s\n", filename);
}

// zapis do pliku binarnego (.bin)
void write_to_bin(Graph* graph, const char* filename) {
    FILE* out = fopen(filename, "wb"); 
    if (!out) {
        printf("Błąd: Nie można otworzyć pliku BIN do zapisu: %s\n", filename);
        return;
    }

    // nagłowek (liczba wierzchołków)
    fwrite(&(graph->num_nodes), sizeof(int), 1, out);

    // dane każdego wierzchołka (id x y)
    for (int i = 0; i < graph->num_nodes; i++) {
        fwrite(&(graph->nodes[i].id), sizeof(int), 1, out);
        fwrite(&(graph->nodes[i].x), sizeof(double), 1, out);
        fwrite(&(graph->nodes[i].y), sizeof(double), 1, out);
    }

    fclose(out);
    printf("Pomyślnie zapisano zrzut pamięci do pliku binarnego: %s\n", filename);
}
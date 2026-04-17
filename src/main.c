#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "shared/config.h"
#include "shared/graph.h"
#include "fruchterman/fruchterman.h"
#include "tutte/tutte.h"
#include "shared/data_loader/data_loader.h"
#include "shared/writer/writer.h"

void print_help() {
    printf("Wywołanie programu:\n");
    printf("-fin <sciezka>     Sciezka do pliku wejsciowego z grafem\n");
    printf("-fout <sciezka>    Sciezka do pliku wyjsciowego\n");
    printf("-bin               Zapis do pliku w formacie binarnym\n");
    printf("-alg <0|1> [...]   Wybór algorytmu:\n");
    printf("                   0 = Fruchterman-Reingold (wymaga: szerokosc, wysokosc, temperatura, iteracje)\n");
    printf("                   1 = Tutte Embedding (wymaga: promien, k wezlow, tryb selekcji)\n");
    printf("-help lub -?       Wypisuje to menu pomocy\n");
}

void setup_default_config(Config * c){
    c->use_binary = false;
    c->algorithm = FR;
    
    c->width = 1.0;
    c->height = 1.0;
    c->temp = 1.0;
    c->iterations = 50;
    
    c->radius = 1.0;
    c->k_fixed = 3;
    c->selection_mode = H_DEG;
}

int main(int argc, char ** argv) {
    Config * c = malloc(sizeof(Config));
    Graph * g = NULL; 

    setup_default_config(c);

    FILE * file_in = NULL;
    char * out_path = "coords.txt"; 

    for (int i = 1; i < argc; i++) {
        // -help lub -?
        if (strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "-?") == 0) {
            print_help();
            free(c);
            return 0;
        } 

        // -bin
        else if (strcmp(argv[i], "-bin") == 0) {
            c->use_binary = true;
        } 

        // -fin
        else if (strcmp(argv[i], "-fin") == 0 && i + 1 < argc) {
            file_in = fopen(argv[i+1], "r");
            if(file_in == NULL) {
                printf("Error: Nie udalo sie zaladowac pliku o nazwie %s\n", argv[i+1]);
            }
            i++;
        } 

        // -fout 
        else if (strcmp(argv[i], "-fout") == 0 && i + 1 < argc) {
            out_path = argv[i+1];
            i++;
        } 

        // -alg
        else if (strcmp(argv[i], "-alg") == 0 && i + 1 < argc) {
            int alg_type = atoi(argv[i+1]);
            
            // 1 = Tutte
            if (alg_type == 1) {
                c->algorithm = TUTTE;
                // Proba pobrania 3 dodatkowych argumentow
                if (i + 4 < argc && argv[i+2][0] != '-' && argv[i+3][0] != '-' && argv[i+4][0] != '-') {
                    c->radius = atof(argv[i+2]);
                    c->k_fixed = atoi(argv[i+3]);
                    c->selection_mode = atoi(argv[i+4]);
                    i += 4;
                } else {
                    i++; // Brak argumentow
                }
            } 
            // 0 = FR (lub jakikolwiek inny numer)
            else {
                c->algorithm = FR;
                // Proba pobrania 4 dodatkowych argumentow
                if (i + 5 < argc && argv[i+2][0] != '-' && argv[i+3][0] != '-' && argv[i+4][0] != '-' && argv[i+5][0] != '-') {
                    c->width = atof(argv[i+2]);
                    c->height = atof(argv[i+3]);
                    c->temp = atof(argv[i+4]);
                    c->iterations = atoi(argv[i+5]);
                    i += 5;
                } else {
                    i++; 
                }
            }
        }
    }

    if (file_in == NULL) {
        printf("Blad: Brak lub zla sciezka pliku wejsciowego.\n");
        free(c);
        return 1;
    }

    g = load_from_file(file_in);
    fclose(file_in);

    if (g == NULL || g->num_nodes == 0) {
        printf("Blad: Graf jest pusty lub uszkodzony.\n");
        if(g) { 
            free(g->edges); 
            free(g->nodes); 
            free(g); 
        }
        free(c);
        return 1;
    }

    if (c->algorithm == FR) {
        fruchterman(g, c); 
    } else {
        run_tutte_embedding(g, c);
    }

    if (c->use_binary) {
        write_to_bin(g, out_path);
    }
    write_to_txt(g, out_path);
    

    free(g->edges);
    free(g->nodes);
    free(g);
    free(c);

    return 0;
}
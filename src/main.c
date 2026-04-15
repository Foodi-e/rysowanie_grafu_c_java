#include <stdlib.h>
#include <stdio.h>
#include "config.h"
#include "graph.h"
#include "fruchterman.h"
#include <stdbool.h>

#define OUTPUT coords.txt

FILE * open_file(char * name, char * type){
    FILE * fptr = fopen(name, type);
    if(fptr == NULL){
        printf("Error: Nie udalo sie zaladowac pliku o nazwie %s \n", name);
    }
    return fptr;
}

void free_main(Graph * g, Config * c){
    free(g->edges);
    free(g->nodes);
    free(g);
    free(c);
}

void load_algorithm_variables(char ** argv, int arg_count, int ind, Config * c){
    if(arg_count < 1){
        return;
    }

    int alg;
    if(sscanf(argv[ind],"%d",&alg)==1){
        if()
    }
}

int check_args(int ind, char**argv, int argc){
    int ret = 0;
    for(int i=ind; i<argc; i++){
        if(argv[i][0]!='-'){
            ret+=1;
        }
    }
    return ret;
}

void setup_default_config(Config * c){
    c->width = 1;
    c->height = 1;
    c->temp = 1;
    c->iterations = 50;
    c->radius = 1;
    c->k_fixed = 3;
    c->selection_mode = CUSTOM;
    c->use_binary = false;
    c->algorithm = FR;
}

int main(int argc, char ** argv){
    Config * c = malloc(sizeof(Config));
    Graph * g = malloc(sizeof(Graph));

    setup_default_config(c);

    FILE * file_out;
    FILE * file_in;

    int i = 1;
    int arg_count;
    while(i<argc){
        arg_count = check_args(i+1, argv, argc);
        switch(argv[i]){
            case "-bin":
                c->use_binary = true;
            case "-fout":
                if(arg_count > 0){file_out = open_file(argv[i+1],"w");}
            case "-fin":
                if(arg_count > 0){file_in = open_file(argv[i+1],"r");}
            case "-alg":
                load_algorithm_variables(argv, arg_count, i+1, c);
        }
        i+=(1+argc);
    }
}


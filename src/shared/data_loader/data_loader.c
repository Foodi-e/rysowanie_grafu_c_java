#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../graph.h"
// definiujemy pewne wielkosci
// MAks Dlugosc jednej lini
// dlugosc elementu - element to jedna z 4 czesci inputu (nazwa, id, id, waga)
// Init size - rozmiar wielkosci tablicy z krawedziami
#define LINE_S 1024
#define EL_S 256
#define INIT_SIZE 128

void add_degree(Graph * g, int * max_size_n){
    int state = 0; // 3 - both 1- two 2- one 0-neither
    for(int i =0; (i<g->num_nodes) && (state !=3); i++ ){
        if(g->nodes[i].id == g->edges[g->num_edges].u){
            g->nodes[i].degree +=1;
            g->edges[g->num_edges].u = i;
            state+=1;
        }
        if(g->nodes[i].id == g->edges[g->num_edges].v){
            g->nodes[i].degree +=1;
            g->edges[g->num_edges].v = i;
            state+=2;
        }
    }
    if((state == 0) || (state == 2)){
        g->nodes[g->num_nodes].degree +=1;
        g->nodes[g->num_nodes].id = g->edges[g->num_edges].u;
        g->edges[g->num_edges].u = g->num_nodes;
        g->num_nodes+=1;
        if(g->num_nodes >= *max_size_n){
            *max_size_n = (*max_size_n) *2;
            g->nodes = realloc(g->nodes, (*max_size_n)*sizeof(Node));
        }
    }
    //jedyna rzecz ktora sie zmieniea to u -> v
    if((state == 0) || (state == 1)){
        g->nodes[g->num_nodes].degree +=1;
        g->nodes[g->num_nodes].id = g->edges[g->num_edges].v;
        g->edges[g->num_edges].v = g->num_nodes;
        g->num_nodes+=1;
        if(g->num_nodes >= *max_size_n){
            *max_size_n = (*max_size_n) *2;
            g->nodes = realloc(g->nodes, (*max_size_n)*sizeof(Node));
        }
    }
}

//funckja ladujaca z pliku, przyjmuje co pointer do otwartego pliku
Graph * load_from_file(FILE * fptr){
    //inicjalizacja buffery dla fgets
    char buffer[LINE_S];
    //zmienne sluzace do sledzenia wielkosci tablicy 
    int max_size_e = INIT_SIZE;
    int max_size_n = INIT_SIZE/2;
    // tablicy krawedzi
    Graph * g = malloc(sizeof(Graph));
    g->num_nodes = 0;
    g->num_edges = 0;
    g->nodes = malloc(sizeof(Node)*max_size_n);
    g->edges = malloc(sizeof(Edge)*max_size_e);
    
    printf("petla start\n");

    //tworzmy format dla scanf - %255s %255s %255s %255s - zeby przez przypadek nie wyszedl poza granice zmiennych podczas zczytywania
    char format[32];
    snprintf(format,sizeof(format), "%%%ds %%%ds %%%ds %%%ds", EL_S - 1,EL_S - 1,EL_S - 1,EL_S - 1);

    //standardowa petla z fgetsem
    while(fgets(buffer,LINE_S,fptr)!=NULL){
        //TUTAJ TRZEBA DODAC TO UNIKALNE ID KRAWEDZI CO TY CHCIALES IMO ZBYT DUZO ROBOTY
        strncpy(g->edges[g->num_edges].name, "BRAK", EL_S-1);
        //inicjalizacja pol dla kazdego elementu inputu
        char element_one[EL_S];
        char element_two[EL_S];
        char element_three[EL_S];
        char element_four[EL_S];
        printf("poczatek petli");
        //szczytujemy elementy z linii
        int n_el = sscanf(buffer,format, element_one, element_two, element_three, element_four);
        //printf("\n nel %d %s %s %s %s \n", n_el,element_one, element_two, element_three, element_four);
        //jesli nie ma jednego z id pomijamy linie
        if((n_el == 0) || (n_el==1)){
            continue;
        }
        // jesli l elementow to dokladnie 2 to znaczy ze musza to byc id wierzcholkow
        // szcyztujemy jeden po drugim sprawdzamy czy liczba calkowita zostala wczytana - jesli nie to pomijamy linie
        else if(n_el==2){
            int number;
            int res = sscanf(element_one,"%d",&number);
            if(res == 1){
                g->edges[g->num_edges].u = number;
            } else { continue; }
            res = sscanf(element_two,"%d",&number);
            if(res == 1){
                g->edges[g->num_edges].v = number;
            } else { continue; }
            add_degree(g,&max_size_n);
        }
        //dla 3 elementow - mamy dwa id i nazwe
        //dla 4 tak samo + waga 
        // ta czesc kodu odpowiada za przepisanie nazwy i obydwu id
        else if((n_el==3) || (n_el==4)){
            int number;
            int res = sscanf(element_two,"%d",&number);
            if(res == 1){
                g->edges[g->num_edges].u = number;
            } else { continue; }
            res = sscanf(element_three,"%d",&number);
            if(res == 1){
                g->edges[g->num_edges].v = number;
            } else { continue; }
            strncpy(g->edges[g->num_edges].name, element_one, EL_S-1);
            add_degree(g,&max_size_n);
        }
        // tutaj nadajemy domyslna wage dla kazdej udanej linii (2, 3, 4 elementy)
        g->edges[g->num_edges].weight = 1;
        
        // tutaj dla 4 elementow sprawdzamy czy da sie odczytac wage - jesli tak zapisujemy
        if(n_el == 4){
            double number;
            int res = sscanf(element_four, "%lf" ,&number);
            if(res == 1){
                g->edges[g->num_edges].weight = number;
            } 
        }
        //jesli linia jest dobra( nie zadzialalo zadne "continue" ) to zwiekszamy obecny rozmiar tablicy krawedzi w strukturze o 1
        g->num_edges+=1;
        //sprawdzamy czy ta tablice trzeba rozszerzyc
        if(g->num_edges >= max_size_e){
            max_size_e = max_size_e *2;
            g->edges = realloc(g->edges, max_size_e*sizeof(Edge));
        }
        //resetujemy elementy
        element_one[0] = '\0';
        element_two[0] = '\0';
        element_three[0] = '\0';
        element_four[0] = '\0';
    }
    g->edges = realloc(g->edges, g->num_edges*sizeof(Edge));
    g->nodes = realloc(g->nodes, g->num_nodes*sizeof(Node));
    return g;
}

//funkcja do wyprintowania powstalej tablicy krawedzi
void print_edge_array(Graph * g){
    printf("printujemy krawedzie\n");
    for(int i=0; i<g->num_edges; i++){
        Edge e = g->edges[i];
        printf(" %s %d %d %lf \n", e.name, e.u, e.v, e.weight);
    }
    printf("printujemy wierzcholki");
    for(int i=0; i<g->num_nodes; i++){
        Node n = g->nodes[i];
        printf(" %d. %d %d \n", i, n.id,n.degree);
    }
}


// main sluzacy do testowania
/*
void main(int argc, char ** argv){
    printf("Zaczynamy");
    Graph * g = load_from_file(fopen(argv[1],"r"));
    printf("Załadowano\n");
    print_edge_array(g);
}*/
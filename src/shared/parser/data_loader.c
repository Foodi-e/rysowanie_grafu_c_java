#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// definiujemy pewne wielkosci
// MAks Dlugosc jednej lini
// dlugosc elementu - element to jedna z 4 czesci inputu (nazwa, id, id, waga)
// Init size - rozmiar wielkosci tablicy z krawedziami
#define LINE_S 1024
#define EL_S 256
#define INIT_SIZE 128

//struktura krawedzi
typedef struct{
    int node_1;
    int node_2;
    double weight;
    char name[EL_S];
} edge;

//struktura tablicy krawedzi
typedef struct{
    edge * array;
    int size;
} edge_array;

//funckja ladujaca z pliku, przyjmuje co pointer do otwartego pliku
edge_array * load_from_file(FILE * fptr){
    //inicjalizacja buffery dla fgets
    char buffer[LINE_S];
    // tablicy krawedzi
    edge_array * arr = malloc(sizeof(edge_array));
    // tablicy krawedzi w strukturze tablica krawedzi
    arr->array = malloc(INIT_SIZE * sizeof(edge));
    //zmienne sluzace do sledzenia wielkosci tablicy 
    int max_size = INIT_SIZE;
    int cur_size = 0;
    printf("petla start\n");

    //tworzmy format dla scanf - %255s %255s %255s %255s - zeby przez przypadek nie wyszedl poza granice zmiennych podczas zczytywania
    char format[32];
    snprintf(format,sizeof(format), "%%%ds %%%ds %%%ds %%%ds", EL_S - 1,EL_S - 1,EL_S - 1,EL_S - 1);

    //standardowa petla z fgetsem
    while(fgets(buffer,LINE_S,fptr)!=NULL){
        //TUTAJ TRZEBA DODAC TO UNIKALNE ID KRAWEDZI CO TY CHCIALES IMO ZBYT DUZO ROBOTY
        strncpy(arr->array[cur_size].name, "BRAK", EL_S-1);
        //inicjalizacja pol dla kazdego elementu inputu
        char element_one[EL_S];
        char element_two[EL_S];
        char element_three[EL_S];
        char element_four[EL_S];
        printf("poczatek petli");
        //szczytujemy elementy z linii
        int n_el = sscanf(buffer,format, element_one, element_two, element_three, element_four);
        printf("\n nel %d %s %s %s %s \n", n_el,element_one, element_two, element_three, element_four);
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
                arr->array[cur_size].node_1 = number;
            } else { continue; }
            res = sscanf(element_two,"%d",&number);
            if(res == 1){
                arr->array[cur_size].node_2 = number;
            } else { continue; }
        }
        //dla 3 elementow - mamy dwa id i nazwe
        //dla 4 tak samo + waga 
        // ta czesc kodu odpowiada za przepisanie nazwy i obydwu id
        else if((n_el==3) || (n_el==4)){
            int number;
            strncpy(arr->array[cur_size].name, element_one, EL_S-1);
            int res = sscanf(element_two,"%d",&number);
            if(res == 1){
                arr->array[cur_size].node_1 = number;
            } else { continue; }
            res = sscanf(element_three,"%d",&number);
            if(res == 1){
                arr->array[cur_size].node_2 = number;
            } else { continue; }
        }
        // tutaj nadajemy domyslna wage dla kazdej udanej linii (2, 3, 4 elementy)
        arr->array[cur_size].weight = 1;
        printf(" dodajemy domyslnie %lf ", arr->array[cur_size].weight);
        
        printf("srodek petli");
        // tutaj dla 4 elementow sprawdzamy czy da sie odczytac wage - jesli tak zapisujemy
        if(n_el == 4){
            double number;
            int res = sscanf(element_four, "%lf" ,&number);
            if(res == 1){
                arr->array[cur_size].weight = number;
                printf(" dodajemy %lf ", arr->array[cur_size].weight);
            } 
        }
        //jesli linia jest dobra( nie zadzialalo zadne "continue" ) to zwiekszamy obecny rozmiar tablicy krawedzi w strukturze o 1
        cur_size+=1;
        //sprawdzamy czy ta tablice trzeba rozszerzyc
        if(cur_size >= max_size){
            max_size = max_size *2;
            arr->array = realloc(arr->array, max_size*sizeof(edge));
        }
        printf("koniec petli");
        //resetujemy elementy
        element_one[0] = '\0';
        element_two[0] = '\0';
        element_three[0] = '\0';
        element_four[0] = '\0';
    }
    arr->size = cur_size;
    return arr;
}

//funkcja do wyprintowania powstalej tablicy krawedzi
void print_edge_array(edge_array * aptr){
    printf("printujemy\n");
    for(int i=0; i<aptr->size; i++){
        edge e = aptr->array[i];
        printf(" %s %d %d %lf \n", e.name, e.node_1, e.node_2, e.weight);
    }
}


// main sluzacy do testowania
/*
void main(int argc, char ** argv){
    printf("Zaczynamy");
    edge_array * arr = load_from_file(fopen(argv[1],"r"));
    printf("Załadowano\n");
    print_edge_array(arr);
}*/
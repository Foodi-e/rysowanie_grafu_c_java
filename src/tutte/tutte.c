#include "tutte.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static int find_highest_degree_unfixed(Graph* graph);
static void setup_boundary(Graph* graph, Config* config);
static void solve_linear_system(double* M, double* b, int n);
static bool is_graph_connected_to_boundary(Graph* graph); 

void run_tutte_embedding(Graph* graph, Config* config) {
    if (graph->num_nodes < 3) {
        printf("Błąd: graf musi mieć conajmniej 3 wierzchołki\n");
        return;
    }
    if (config->k_fixed < 3) {
        printf("Błąd: Tutte wymaga conajmniej 3 stałe wierzchołki\n");
        return;
    }
    //ustawianie na początek wszystkie wierzchołki na pewno nie są ramką
    for (int i = 0; i < graph->num_nodes; i++) {
        graph->nodes[i].is_fixed = false;
    }

    //wyznaczanie ktore wierzchołki są ramką
    setup_boundary(graph, config);

    if (!is_graph_connected_to_boundary(graph)) {
        printf("Błąd: Graf jest niespójny! Część wierzchołków lewituje w przestrzeni\n");
        printf("Przerwano algorytm Tuttego, aby zapobiec dzieleniu przez zero w macierzy\n");
        return;
    }

    int num_internal = 0; // ilość wierzchołkow ktore nie są stałe

    //tablica w ktorej kazdy wierzchołek node_to_matrix[i] ma swoją wartość w macierzy
    //macierz nie jest tego samego rozmiaru co liczba wierzchołkow, jesk o k mniejsza 
    int* node_to_matrix = malloc(graph->num_nodes * sizeof(int));  

    //zapełniamy tablice node_to_matrix
    for (int i = 0; i < graph->num_nodes; i++) {
        if (!graph->nodes[i].is_fixed) {
            node_to_matrix[i] = num_internal;
            num_internal++;
        } else {
            node_to_matrix[i] = -1; // ten wierzchołek jest stały a więc jego pozycja w macierzy nie istnieje 
        }
    }

    //jeśli wszystkie wierzchołki są stałe to koniec
    if (num_internal == 0) {
        free(node_to_matrix);
        return;
    }

    //tworze dwie macierzy i dwa wektory dla układow rownań, dla macierzy M nie uzywam 
    //dwuwymiarowej tablicy bo podobno tak jest szybciej, calloc od razu nadaje kazdemu elementu wartość 0.0
    double* M_x = calloc(num_internal * num_internal, sizeof(double));
    double* M_y = calloc(num_internal * num_internal, sizeof(double));
    double* b_x = calloc(num_internal, sizeof(double));
    double* b_y = calloc(num_internal, sizeof(double));

    //zapełniam danymi macierz
    for (int i = 0; i < graph->num_edges; i++) {
        int u = graph->edges[i].u;
        int v = graph->edges[i].v;
        double w = graph->edges[i].weight;

        int row_u = node_to_matrix[u];
        int row_v = node_to_matrix[v];

        // wierz U, kolumna V
        if (row_u != -1) { // sprawdzenie czy wierzchołek U jest ruchomy
            int u_u = row_u * num_internal + row_u; // wiersz U, kolumna U macierzy (U, U)
            
            //zwiękaszanie wagi na diagonali macierzy (na koniec otrzymamy sumę wag sąsiadow U)
            //całkowita siła, z jaką wszystkie sprężyny trzymają go w miejscu
            M_x[u_u] += w;
            M_y[u_u] += w;

            if (row_v != -1) { // jeśli wierzchołek V tez jest ruchomy 
                int u_v = row_u * num_internal + row_v; // wiersz U, kolumna V macierzy: (U, V)

                //wierzchołek V przyciąga wierzchołek U z siła w, czyli U się przyciąga to wierzchołka V z siła -w
                M_x[u_v] -= w;
                M_y[u_v] -= w;

            } else { // jeśli wierzchołek V tez jest stały 
                //jak V jest stały to U nie moze wpływać na pozycję V 
                b_x[row_u] += w * graph->nodes[v].x;
                b_y[row_u] += w * graph->nodes[v].y;
            }
        }

        // wiersz V, kolumna U (wszystko tak samo jak dla U, tylko od strony V)
        if (row_v != -1) {
            int v_v = row_v * num_internal + row_v;
            M_x[v_v] += w;
            M_y[v_v] += w;

            if (row_u != -1) {
                int v_u = row_v * num_internal + row_u;
                M_x[v_u] -= w;
                M_y[v_u] -= w;
            } else {
                b_x[row_v] += w * graph->nodes[u].x;
                b_y[row_v] += w * graph->nodes[u].y;
            }
        }
    }
    

        //rozwiązanie macierzy, po zakonczeniu bx i by beda zawierac wspolrzedne koncowe wierzcholkow
        solve_linear_system(M_x, b_x, num_internal);
        solve_linear_system(M_y, b_y, num_internal);

        // dwa pointery, jeden(matrix_row) to pointer na odpowiedzi, drugi("i") to pointer na wierzchołki
        //potrzebne dwa pointery bo niektore wierzchołki są stałe i trzeba je ignorawać
        int matrix_row = 0; // Pointer 1
        for (int i = 0; i < graph->num_nodes; i++) { // Pointer 2 'i'
            if (!graph->nodes[i].is_fixed) {//przepisywanie tylko dla ruchomych wierzchołkow
                // przepisuje wyniki zawarte w wektorach do tablicy wierzchołkow
                graph->nodes[i].x = b_x[matrix_row];
                graph->nodes[i].y = b_y[matrix_row];
                
                matrix_row++; 
            }
        }
    
    free(node_to_matrix);
    free(M_x);
    free(M_y);
    free(b_x);
    free(b_y);
}

//szukamy wierzchołka ktory ma najwięcej sąsiadow (degree)
static int find_highest_degree_unfixed(Graph* graph) {
    int max_deg = -1;
    int best_idx = -1;//indeks wierzchołka z największą ilością sąsiadow
    
    //petla przeszukuje tablice wierzchołkow ktore nie są ramką(outer face)
    for (int i = 0; i < graph->num_nodes; i++) {
        if (!graph->nodes[i].is_fixed && graph->nodes[i].degree > max_deg) {
            max_deg = graph->nodes[i].degree;
            best_idx = i;
        }
    }
    return best_idx;
}

//nadajemy wierzchołkom stałej pozycji na okręgu(ramka)
static void setup_boundary(Graph* graph, Config* config) {

    int k = config->k_fixed;

    //sprawdzenie czy ilość wierzchołkow ktore uzytkownik wkazaw nie jest większa od rzeczywistej
    if (k > graph->num_nodes) {
        k = graph->num_nodes; 
    }

    //tablica pod ramkę
    int* boundary_nodes = malloc(k * sizeof(int));

    if (config->selection_mode == CUSTOM) { // pierwsze k wierzchołkow z pliku są ramką(fixed)
        for (int i = 0; i < k; i++) {
            boundary_nodes[i] = i;
            graph->nodes[i].is_fixed = true;
        }
    } else { // k wierzchołow o najwyszym degree są ramką
        for (int i = 0; i < k; i++) {
            int best_idx = find_highest_degree_unfixed(graph);
            if (best_idx != -1) { 
                boundary_nodes[i] = best_idx;
                graph->nodes[best_idx].is_fixed = true;

            } else { // w ogole nie powinno sie trigerować bo wyzej mamy sprawdzenie k > num_nodes
                printf("Uwaga: Wpisano %d stałych wierzchołkow, ale znaleziono tylko %d.\n", k, i);
                k = i;
                break;
            }
        }
    }

    //rozmieszczanie wierzchołkow na okręgu
    for (int i = 0; i < k; i++) {
        int idx = boundary_nodes[i];
        double angle = 2.0 * M_PI * i / k; // kąt w radianach
        graph->nodes[idx].x = config->radius * cos(angle);
        graph->nodes[idx].y = config->radius * sin(angle);
    }

    free(boundary_nodes);
}

//eliminacja gaussa
static void solve_linear_system(double* M, double* b, int n){
    for (int k = 0; k < n; k++) {
        int pivot_idx = k * n + k; //wiersz K, kolumna K
        double pivot = M[pivot_idx]; //liczba na przekątnej macierzy
        
        // dzielnie całego wierszu macierzy przez pivot 
        for (int j = k; j < n; j++) {
            int current_idx = k * n + j; // wiersz K, kolumna J
            M[current_idx] /= pivot;
        }
        b[k] /= pivot;//wektor z odpowiedziami (musimy podzielić lewą i prawą stronę rownania)

        // elimincja elementow pod elementem k (schodki)
        for (int i = k + 1; i < n; i++) {
            
            int factor_idx = i * n + k;  // wiersz i, kolumna k 
            double factor = M[factor_idx]; 
            
            // odejmuje wiersze
            for (int j = k; j < n; j++) {
                int target_idx = i * n + j;  // wiersz i, kolumna j
                int source_idx = k * n + j;  // wiersz k, kolumna j
                
                //target - wiersz w ktorym muszą wyjść zera
                M[target_idx] -= factor * M[source_idx];
            }
            b[i] -= factor * b[k]; //to samo co dla macierzy tylko dla wektora
        }
    }

    for (int i = n - 1; i >= 0; i--) {
        for (int j = i + 1; j < n; j++) {
            int cell_idx = i * n + j; 
            b[i] -= M[cell_idx] * b[j]; 
        }
    }
}

// Sprawdza czy każdy wierzchołek ma ścieżkę do ramki (boundary)
static bool is_graph_connected_to_boundary(Graph* graph) {
    bool* visited = calloc(graph->num_nodes, sizeof(bool));
    int visited_count = 0;

    // zaznaczamy wszystkie stałe wierzchołki (ramkę) jako odwiedzone(pomalowane)
    for (int i = 0; i < graph->num_nodes; i++) {
        if (graph->nodes[i].is_fixed) {
            visited[i] = true;
            visited_count++;
        }
    }

    // jeśli nie ma ramki, z automatu nie możemy kontynuować
    if (visited_count == 0) {
        free(visited);
        return false;
    }

    // rozprzestrzeniamy "farbę" po krawędziach
    bool changed;
    do {
        changed = false;
        for (int i = 0; i < graph->num_edges; i++) {
            int u = graph->edges[i].u;
            int v = graph->edges[i].v;

            // jeśli u jest połączony z ramką, a v jeszcze nie - "pomaluj" v
            if (visited[u] && !visited[v]) {
                visited[v] = true;
                visited_count++;
                changed = true;
            } 
            // w drugą stronę, jeśli v jest połączony, a u nie - "pomaluj" u
            else if (visited[v] && !visited[u]) {
                visited[u] = true;
                visited_count++;
                changed = true;
            }
        }
    } while (changed);

    // jeśli liczba pomalowanych wierzchołków równa się wszystkim, graf jest spójny
    bool is_connected = (visited_count == graph->num_nodes);
    free(visited);
    
    return is_connected;
}
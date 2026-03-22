#include <stdlib.h>
#include <stdio.h>
#include "config.h"
#include "graph.h"
#include <math.h>
#include <time.h>

typedef struct{
    double x;
    double y;
}vec2;

void loop(Graph * g, Config * c, double k, vec2 * va, double * temp);

void Fruchterman(Graph * g, Config * c){
    srand( time( NULL ) );

    double temp = c->temp;
    //losujemy pozycje poczatkowe wierzcholkow
    for(int i=0;(i<g->num_nodes) || (temp - 0.0001 < 0); i++){
        g->nodes[i].x = (rand()/(RAND_MAX+1.0)) * c->width;
        g->nodes[i].y = (rand()/(RAND_MAX+1.0)) * c->height;
    }
    // create velocity array
    vec2 * va = malloc(sizeof(vec2) * g->num_nodes);
    // stala k
    double k = sqrt(c->width*c->height /c->iterations);
    for(int i=0; (i<c->iterations) || (temp - 0.00001 < 0.0); i++){
        loop(g, c, k, va, &temp);
    }
}

void loop(Graph * g, Config * c, double k, vec2 * va, double * temp){
    //Liczymy sily odpychu
    for(int i=0; i<g->num_nodes; i++){
        for(int j=0; i<g->num_nodes-i-1; j++){
            vec2 force = (vec2){0,0};
            double length;
            force.x = g->nodes[i].x - g->nodes[j+i+1].x;
            force.y = g->nodes[i].y - g->nodes[j+i+1].y;
            length = sqrt(force.x*force.x + force.y*force.y);
            if(length - 0.00001 < 0){
                continue;
            }
            force.x = (force.x / length) * ((k*k)/length);
            force.y = (force.y / length)  * ((k*k)/length);

            va[i].x += force.x;
            va[i].y += force.y;

            va[j+i+1].x -= force.x;
            va[j+i+1].y -= force.y;
        }
    }
    //liczymy sily przyciagania
    for(int i=0; i<g->num_edges; i++){
        vec2 force = (vec2){0,0};
        double length;
        force.x = g->nodes[g->edges[i].u].x - g->nodes[g->edges[i].v].x;
        force.y = g->nodes[g->edges[i].u].y - g->nodes[g->edges[i].v].y;
        length = sqrt(force.x*force.x + force.y*force.y);
        force.x = (force.x / length) * (g->edges[i].weight * length*length) / k;
        force.y = (force.y / length) * (g->edges[i].weight * length*length) / k;

        g->nodes[g->edges[i].u].x -= force.x;
        g->nodes[g->edges[i].u].y -= force.y;

        g->nodes[g->edges[i].v].x += force.x;
        g->nodes[g->edges[i].v].y += force.y;
    }
    //przesuwamy
    for(int i = 0; i<g->num_nodes; i++){
        double length = sqrt(va[i].x*va[i].x + va[i].y*va[i].y);
        if(length - 0.00001 < 0){
            va[i] = (vec2){0,0};
            continue;
        }
        va[i].x = (va[i].x/length)*fmin(length,*temp);
        va[i].y = (va[i].y/length)*fmin(length,*temp);
        g->nodes[i].x += va[i].x;
        g->nodes[i].y += va[i].y;
        va[i] = (vec2){0,0};

        
        if (g->nodes[i].x < 0.0){
            g->nodes[i].x = 0.0;
        }    
        else if (g->nodes[i].x > c->width){
            g->nodes[i].x = c->width;
        }

        if (g->nodes[i].y < 0.0){
            g->nodes[i].y = 0.0;
        }    
        else if (g->nodes[i].y > c->height){
            g->nodes[i].y = c->height;
        }
    }
    *temp = 0.9*(*temp);
}

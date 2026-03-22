#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

enum Algorithm{     // -alg
    FR = 0,
    TUTTE = 1
};

enum Selection_mode{
    CUSTOM = 0,
    H_DEG = 1 //highest degree
};

typedef struct {
    char *input_path;   // -fin
    char *output_path;  // -fout 
    bool use_binary;    // -bin
    enum Algorithm algorithm;
    //FR
    double width, height, temp;
    int iterations; //nie wiem czy tego będziesz potrzebował, bo chyba nie wspominałeś podczas prezentacji

    //tutte
    double radius;
    int k_fixed;
    enum Selection_mode selection_mode;
} Config;

#endif
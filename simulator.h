#ifndef SIMULATOR_H_
#define SIMULATOR_H_ 

#include <stdio.h>

#define MEM_ACCESS_MAX 1000005 /* OBS: 2m aqui causa segfault (provavelmente por overflow no heap)! */

typedef struct Simulator Simulator;

typedef enum {
    RANDOM, /* Escolhe as páginas aleatóriamente. */
    NRU,    /* Non-Recently Used */
    LRU,    /* Least Recently Used */
    SEC     /* Second Chance */
} Algorithm; /* Algoritmo de Page Replacement (substituição de página). */

typedef struct Options {
    char *filename;
    Algorithm algo;
    int p_size_kb, phys_mem_kb;
    int debug_mode;
} Options;

Algorithm AlgorithmFromString(char *str);
Options OptionsFromArgs(int argc, char const *argv[]);

Simulator *SimulatorInit(Simulator *sim, FILE *fp);

void SimulatorRun(Simulator *sim, Options opts);

void SimulatorPrint(Simulator *sim);
// void SimulatorLoad(FILE *fp, Simulator *sim);
int SimulatorLength(Simulator *sim);
void SimulatorPrintResults(Simulator *sim);
void SimulatorDestroy(Simulator *sim);

#endif

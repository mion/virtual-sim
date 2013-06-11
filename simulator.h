#ifndef SIMULATOR_H_
#define SIMULATOR_H_ 

#include <stdio.h>

#define MEM_ACCESS_MAX 2000

typedef struct MemAccess {
    char rw;
    unsigned addr;
} MemAccess;

typedef struct Simulator {
    MemAccess mem_accesses[MEM_ACCESS_MAX];
    int size;
} Simulator;

void MemAccessPrint(MemAccess mem_access);
void SimulatorPrint(Simulator *sim);
void SimulatorLoad(FILE *fp, Simulator *sim);

#endif

#ifndef SIMULATOR_H_
#define SIMULATOR_H_ 

#include <stdio.h>

#define MEM_ACCESS_MAX 1000005 /* OBS: 2m aqui causa segfault (provavelmente por overflow no heap)! */

typedef struct MemAccess {
    char rw;
    unsigned addr;
} MemAccess;

struct Simulator {
    MemAccess mem_accesses[MEM_ACCESS_MAX];
    char alg[31];
    int length, p_size_kb, phys_mem_kb, debug_mode;
};

typedef struct Simulator Simulator;

void SimulatorPrint(Simulator *sim);
void SimulatorLoad(FILE *fp, Simulator *sim);
int SimulatorLength(Simulator *sim);
void SimulatorRun(Simulator *sim, int options);
void SimulatorPrintResults(Simulator *sim);

#endif

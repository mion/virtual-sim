#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simulator.h"

void tab(int n) {
    while (n--) printf("\t");
}

int main(int argc, char const *argv[]) {
    int i, j, k, writes, reads, p[] = {4, 8, 12, 16, 20, 24, 28, 32};
    const char *filenames[] = {"compilador.log", 
                               "compressor.log", 
                               "simulador.log",
                               "matriz.log"};
    const char *algos[] = {"LRU", "NRU", "SEG"};
    Options opts;
    Simulator *sim; 

    printf("{\n");
    for (i = 0; i < 4; i++) {
        printf("\"%s\": [", filenames[i]);
        for (j = 0; j < 3; j++) {
            strcpy(opts.filename, filenames[i]);
            strcpy(opts.algo, algos[j]);
            opts.phys_mem_kb = 128;
            opts.debug_mode = 0;

            printf("{\n");
            tab(1); printf("\"algoritmo\": \"%s\",\n", algos[j]);

            tab(1); printf("\"resultado\": [\n");
            for (k = 0; k < 8; k++) {
                opts.p_size_kb = p[k]; 

                sim = SimulatorInit(opts);
                SimulatorRun(sim);
                SimulatorResult(sim, &writes, &reads);

                tab(2); printf("{\"tamanho\": %d, ", p[k]);
                printf("\"writes\": %d, ", writes);
                printf("\"reads\": %d}", reads); if (k != 7) printf(",");
                printf("\n");

                SimulatorDestroy(sim);
            }
            
            tab(1); printf("]\n");
            printf("}"); if (j != 2) printf(",\n");
        }
        printf("]"); if(i != 3) printf(",");
        printf("\n");
    }
    printf("}\n");

    return 0;
}
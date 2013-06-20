#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "simulator.h"

#define ARGV_MAX 101

void test(const char *msg, int expr) {
    printf("* TEST: %s\n", msg); 
    assert(expr);
    printf("==> Success!\n\n");
}

void load(char *filename_in, Simulator *sim) {
    FILE *fp = (FILE *) fopen(filename_in, "r");
    if (!fp) {
        printf("ERRO: nao foi possivel abrir o arquivo '%s'.\n", filename_in);
        exit(1);
    }
    SimulatorLoad(fp, sim);
    fclose(fp);
}

int main(int argc, char const *argv[]) {
    Simulator sim;

    printf("===== UNIT TESTS ====\n\n");

    /* Carrega o simulador com o arquivo de entrada. */
    load("file.log", &sim);

    sim.p_size_kb = 4;
    sim.phys_mem_kb = 16;
    sim.debug_mode = 0;

    test("Loaded input file", SimulatorLength(&sim) == 7);

    SimulatorRun(&sim, 0);
    SimulatorPrintResults(&sim);

    return 0;
}

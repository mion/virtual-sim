#ifndef SIMULATOR_H_
#define SIMULATOR_H_ 

#define MEM_ACCESS_MAX 1000005 /* OBS: 2m aqui causa segfault (provavelmente por overflow no heap)! */
#define ARGV_MAX 301

typedef struct Simulator Simulator;

typedef struct Options {
    char filename[ARGV_MAX];
    char algo[ARGV_MAX];
    int p_size_kb, 
        phys_mem_kb;
    int debug_mode;
} Options;

/* Faz um parsing dos argumentos passados na linha de comando
e returna uma struct que representa as opções. */
Options OptionsFromArgs(int argc, char const *argv[]);
/* Imprime as opções, apenas para conferir. */
void OptionsPrint(Options opts);

/* Inicializa o simulador com as opções escolhidas, carregando
os acessos de um arquivo de entrada. */
Simulator *SimulatorInit(Options opts);

/* Executa a simuação. */
void SimulatorRun(Simulator *sim);

/* Imprime o resultado da simulação. */
void SimulatorPrintResult(Simulator *sim);

/* Desaloca recursos utilizados pelo simulador. */
void SimulatorDestroy(Simulator *sim);

// void SimulatorPrint(Simulator *sim);
// void SimulatorLoad(FILE *fp, Simulator *sim);
// int SimulatorLength(Simulator *sim);
// void SimulatorPrintResults(Simulator *sim);
// void SimulatorDestroy(Simulator *sim);

#endif

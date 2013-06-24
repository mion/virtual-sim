#ifndef MEMORY_H_
#define MEMORY_H_ 

/* Estatísticas sobre a/o resultado da simulação. */
typedef struct Statistics
{
    int writes_to_disk, /* Número de escritas ao disco (páginas escritas) */
        page_faults; /* Número de page faults (páginas lidas) */
} Statistics;

/* Representa o estado interno da memória. */
typedef struct Memory Memory;

Memory* MemoryInit(char *algo, int p_size_kb, int phys_mem_kb);

void MemoryDestroy(Memory *mem);

void MemoryClockInterrupt(Memory *mem);

/* Simula um acesso a memória, tanto de leitura como escrita (indicado por rw). */
void MemoryAccess(Memory *mem, unsigned addr, char rw);

void MemoryPrintFrames(Memory *mem);

Statistics MemoryStatistics(Memory *mem);

int MemoryTime(Memory *mem);

#endif

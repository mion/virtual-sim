#ifndef MEMORY_H_
#define MEMORY_H_ 

/* Estatísticas sobre a/o resultado da simulação. */
typedef struct Statistics Statistics;
/* Representa o estado interno da memória. */
typedef struct Memory Memory;

Memory* MemoryInit(int p_size_kb, int phys_mem_kb);
void MemoryDestroy(Memory *mem);
void MemoryClockInterrupt(Memory *mem);
void MemoryAccess(Memory *mem, unsigned addr, char rw);
void MemoryPrintFrames(Memory *mem);

#endif

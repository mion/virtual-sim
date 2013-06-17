#ifndef MEMORY_H_
#define MEMORY_H_ 

void MemoryInit(int p_size_kb, int phys_mem_kb);
void MemoryClockInterrupt(void);
void MemoryAccess(unsigned addr, char rw);
void MemoryStatistics(int* n_writes, int* n_pfaults);

#endif

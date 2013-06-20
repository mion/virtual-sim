#ifndef MEMORY_H_
#define MEMORY_H_ 

void MemoryInit(int p_size_kb, int phys_mem_kb);
void MemoryClockInterrupt(void);
void MemoryAccess(unsigned addr, char rw);
void MemoryStatistics(int* n_writes, int* n_pfaults, int* num_vir_pages, int* num_p_frames);
void MemoryDestroy(void);
void MemoryPrintFrames(void);

#endif

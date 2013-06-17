#ifndef MEMORY_H_
#define MEMORY_H_ 

void MemoryInit(int p_size_kb, int phys_mem_kb);
void MemoryClockInterrupt(void);
void MemoryRead(unsigned addr);
void MemoryWrite(unsigned addr);

#endif

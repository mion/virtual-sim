#ifndef MEMORY_H_
#define MEMORY_H_ 

typedef struct Page
{
    int r_flag, m_flag, last_access_counter;
} Page;

typedef struct VirtualMem
{
    Page *p_table;
    int size;
} VirtualMem;

#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TRUE 1
#define FALSE 0

#define ARGV_MAX 101
#define MEM_ACCESS_MAX 2000

#define IS_PAGE_FAULT(page) (page == NULL)

/* Retorna 2 elevado à n. */
#define pow2(x) (1u<<(x))

unsigned lg2(unsigned x) {
    unsigned r = 0u;
    while(x > 1u) {
       x = x >> 1u; 
       r++;
    }
    return r; 
}

typedef struct MemAccess {
    char rw;
    unsigned addr;
} MemAccess;

typedef struct SimulatorInput {
    MemAccess mem_accesses[MEM_ACCESS_MAX];
    int size;
} SimulatorInput;

void load(FILE *fp, SimulatorInput *sim_input) {
    int i = 0;
    unsigned addr;
    char rw;

    assert(fp);
    assert(sim_input);

    while(fscanf(fp, "%x %c ", &addr, &rw) == 2) {
        if (i >= MEM_ACCESS_MAX) {
            printf("ERROR: ultrapassado numero maximo de linhas no arquivo de log.\n");
            exit(EXIT_FAILURE);
        }
        sim_input->mem_accesses[i].addr = addr;
        sim_input->mem_accesses[i].rw = rw;
        i++;
    }

    sim_input->size = i;

    return;
}

void MemAccess_print(MemAccess mem_access) {
    printf("0x%08x - %c\n", mem_access.addr, mem_access.rw);
}

void SimulatorInput_print(SimulatorInput *sim_input) {
    int i;

    assert(sim_input);

    for (i = 0; i < sim_input->size; i++) {
        MemAccess_print(sim_input->mem_accesses[i]);
    }
}

typedef struct Page
{
    int r_flag, m_flag, last_access_counter;
} Page;

typedef struct VirtualMem
{
    Page *p_table;
    int size;
} VirtualMem;


/* * * * * * * *
 * vir_to_p
 *
 * Retorna o índice de uma página baseado em um endereço lógico.
 * Por exemplo,
 */
unsigned vir_to_p(unsigned addr, unsigned p_size_kb) {
    assert(p_size_kb > 0);

    return addr >> (lg2(p_size_kb) + 10u);
}

void print_usage(void) {

}

void parse_args(int argc, 
               char *argv[], 
               char *alg, 
               char *filename_in, 
               int *p_size_kb, 
               int *phys_mem_kb,
               int *debug_mode) {

    if (argc >= 5) {
        strcpy(alg, argv[1]);
        strcpy(filename_in, argv[2]);
        *p_size_kb = atoi(argv[3]);
        *phys_mem_kb = atoi(argv[4]);
        if (argc >= 6) {
            *debug_mode = atoi(argv[5]);
        }
    }
    else {
        print_usage();
        exit(EXIT_FAILURE); 
    }
}

int main(int argc, char *argv[]) {
    char alg[ARGV_MAX], filename_in[ARGV_MAX];
    int p_size_kb, phys_mem_kb, debug_mode = 0;
    FILE *fp;
    SimulatorInput sim_input;

    parse_args(argc, argv, alg, filename_in, &p_size_kb, &phys_mem_kb, &debug_mode);

    fp = (FILE *) fopen(filename_in, "r");
    if (!fp) {
        printf("ERRO: nao foi possivel abrir o arquivo '%s'.\n", filename_in);
        exit(1);
    }
    load(fp, &sim_input);
    fclose(fp);

    printf("Executando o simulador...\n");
    printf("Arquivo de entrada: %s\n", filename_in);
    printf("Tamanho da memoria: %d\n", phys_mem_kb);
    printf("Tamanho das paginas: %d\n", p_size_kb);
    printf("Alg de substituicao: %s\n", alg);
    printf("Numero total de acessos a memoria: %d\n", sim_input.size);

    SimulatorInput_print(&sim_input); 

    return 0;
}

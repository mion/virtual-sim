#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARGV_MAX 101
#define MEM_ACCESS_MAX 2000

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

struct MemAccess
{
    char rw;
    unsigned addr;
};

typedef struct MemAccess MemAccess;

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

int MemAccess_load(FILE *fp, MemAccess *mem_access) {
    int i = 0;
    unsigned addr;
    char rw;

    assert(fp);
    assert(mem_access);

    while(fscanf(fp, "%x %c ", &addr, &rw) == 2) {
        if (i >= MEM_ACCESS_MAX) {
            printf("ERROR: ultrapassado numero maximo de linhas no arquivo de log.\n");
            exit(EXIT_FAILURE);
        }
        mem_access[i].addr = addr;
        mem_access[i].rw = rw;
        i++;
    }

    return i;
}

void MemAccess_print(MemAccess mem_access) {
    printf("0x%08x - %c\n", mem_access.addr, mem_access.rw);
}

void MemAccess_print_all(MemAccess *mem_access, int size) {
    int i;

    assert(mem_access);
    assert(size >= 0);

    for (i = 0; i < size; i++) {
        MemAccess_print(mem_access[i]);
    }
}

/* * * * * * * *
 * vir_to_p
 *
 * Retorna o índice de uma página baseado em um endereço lógico.
 * Por exemplo,
 */
unsigned vir_to_p(unsigned addr, unsigned p_size_kb) {
    return addr >> (lg2(p_size_kb) + 10u);
}

int main(int argc, char *argv[]) {
    char alg[ARGV_MAX], filename_in[ARGV_MAX];
    int p_size_kb, phys_mem_kb, debug_mode = 0;
    FILE *fp;
    MemAccess mem_access[MEM_ACCESS_MAX];
    int mem_access_size;

    parse_args(argc, argv, alg, filename_in, &p_size_kb, &phys_mem_kb, &debug_mode);

    fp = (FILE *) fopen(filename_in, "r");
    if (!fp) {
        printf("ERRO: nao foi possivel abrir o arquivo '%s'.\n", filename_in);
        exit(1);
    }
    mem_access_size = MemAccess_load(fp, mem_access);
    fclose(fp);

    printf("Executando o simulador...\n");
    printf("Arquivo de entrada: %s\n", filename_in);
    printf("Tamanho da memoria: %d\n", phys_mem_kb);
    printf("Tamanho das paginas: %d\n", p_size_kb);
    printf("Alg de substituicao: %s\n", alg);
    printf("Numero total de acessos a memoria: %d\n", mem_access_size);

    MemAccess_print_all(mem_access, mem_access_size);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "mymemory.h"

mymemory_t* mymemory_init(size_t size) {
    // Aloca a estrutura de controle
    mymemory_t* memory = (mymemory_t*)malloc(sizeof(mymemory_t));
    if (!memory) {
        return NULL;
    }

    // Aloca o pool de memória
    memory->pool = malloc(size);
    if (!memory->pool) {
        free(memory);
        return NULL;
    }

    memory->total_size = size;
    memory->head = NULL;

    return memory;
}

void* mymemory_alloc(mymemory_t *memory, size_t size) {
    if (!memory || !size)
        return NULL;
    
    allocation_t *curr = memory->head;
    allocation_t *prev = NULL;
    void *start = memory->pool;

    // Variáveis para rastrear o melhor ajuste
    void *best_fit_start = NULL;
    size_t best_fit_size = memory->total_size + 1; // Inicializa com valor maior que possível
    allocation_t *best_fit_prev = NULL;

    // Procura o melhor ajuste entre todas as lacunas
    while (curr || start < (char*)memory->pool + memory->total_size) {
        size_t available_size;

        if (curr) {
            available_size = (char*)curr->start - (char*)start;
        } else {
            available_size = (char*)memory->pool + memory->total_size - (char*)start;
        }

        // Se o espaço é suficiente e menor que o melhor ajuste atual
        if (available_size >= size && available_size < best_fit_size) {
            best_fit_size = available_size;
            best_fit_start = start;
            best_fit_prev = prev;
        }

        if (curr) {
            start = (char*)curr->start + curr->size;
            prev = curr;
            curr = curr->next;
        } else {
            break;
        }
    }

    // Se não encontrou espaço adequado
    if (!best_fit_start) {
        return NULL;
    }

    // Cria novo nó de alocação
    allocation_t *new_alloc = (allocation_t*)malloc(sizeof(allocation_t));
    if (!new_alloc) {
        return NULL;
    }

    new_alloc->start = best_fit_start;
    new_alloc->size = size;

    // Insere na lista mantendo a ordem por endereço
    if (!best_fit_prev) {
        new_alloc->next = memory->head;
        memory->head = new_alloc;
    } else {
        new_alloc->next = best_fit_prev->next;
        best_fit_prev->next = new_alloc;
    }

    return best_fit_start;
}

void mymemory_free(mymemory_t *memory, void *ptr) {
    if (!memory || !ptr) {
        return;
    }

    allocation_t *curr = memory->head;
    allocation_t *prev = NULL;

    while (curr && curr->start != ptr) {
        prev = curr;
        curr = curr->next;
    }

    if (curr) {
        if (prev) {
            prev->next = curr->next;
        } else {
            memory->head = curr->next;
        }
        free(curr);
    }
}

void mymemory_display(mymemory_t *memory) {
    if (!memory) {
        printf("Gerenciador de memória inválido\n");
        return;
    }

    printf("\nAlocações de Memória:\n");
    printf("Tamanho Total do Pool: %zu bytes\n", memory->total_size);

    allocation_t *curr = memory->head;
    int bloco = 1;
    void *last_end = memory->pool;

    while (curr) {
        size_t offset = (char*)curr->start - (char*)memory->pool;
        size_t gap = (char*)curr->start - (char*)last_end;

        if (gap > 0) {
            printf("Espaço Livre: offset=%zu tamanho=%zu\n", 
                   (char*)last_end - (char*)memory->pool, gap);
        }

        printf("Bloco %d: offset=%zu tamanho=%zu\n", 
               bloco++, offset, curr->size);

        last_end = (char*)curr->start + curr->size;
        curr = curr->next;
    }

    // Mostra último espaço livre, se houver
    size_t final_gap = (char*)memory->pool + memory->total_size - (char*)last_end;
    if (final_gap > 0) {
        printf("Espaço Livre Final: offset=%zu tamanho=%zu\n",
               (char*)last_end - (char*)memory->pool, final_gap);
    }

    printf("\n");
}

void mymemory_stats(mymemory_t *memory) {
    if (!memory) {
        printf("Gerenciador de memória inválido\n");
        return;
    }

    size_t total_alocado = 0;
    size_t maior_bloco_livre = 0;
    int num_alocacoes = 0;
    int num_fragmentos = 0;
    void *posicao_atual = memory->pool;
    allocation_t *curr = memory->head;

    while (curr) {
        size_t gap = (char*)curr->start - (char*)posicao_atual;
        if (gap > 0) {
            num_fragmentos++;
            if (gap > maior_bloco_livre) {
                maior_bloco_livre = gap;
            }
        }

        total_alocado += curr->size;
        num_alocacoes++;
        posicao_atual = (char*)curr->start + curr->size;
        curr = curr->next;
    }

    size_t gap_final = (char*)memory->pool + memory->total_size - (char*)posicao_atual;
    if (gap_final > 0) {
        num_fragmentos++;
        if (gap_final > maior_bloco_livre) {
            maior_bloco_livre = gap_final;
        }
    }

    printf("\nEstatísticas de Memória:\n");
    printf("Total de alocações: %d\n", num_alocacoes);
    printf("Memória total alocada: %zu bytes\n", total_alocado);
    printf("Memória total livre: %zu bytes\n", memory->total_size - total_alocado);
    printf("Maior bloco livre: %zu bytes\n", maior_bloco_livre);
    printf("Número de fragmentos: %d\n\n", num_fragmentos);
}

void mymemory_cleanup(mymemory_t *memory) {
    if (!memory) {
        return;
    }

    allocation_t *curr = memory->head;
    while (curr) {
        allocation_t *next = curr->next;
        free(curr);
        curr = next;
    }

    free(memory->pool);
    free(memory);
}
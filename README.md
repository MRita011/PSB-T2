# Trabalho 2 - Programação de Software Básico

## Introdução

Este repositório contém o código para o segundo trabalho da disciplina de Programação de Software Básico. O objetivo é desenvolver uma biblioteca simples de gerenciamento de memória em C, que realiza alocações e liberações em um pool de memória predefinido. A estrutura de dados usada organiza as regiões de memória alocadas, permitindo a criação de um gerenciador de memória customizado.

## Especificações

### Estruturas de Dados

1. **`allocation_t`**: Representa um bloco de memória alocado. Contém:
   - `start`: Ponteiro para o início do bloco.
   - `size`: Tamanho do bloco alocado.
   - `next`: Ponteiro para o próximo bloco alocado, formando uma lista encadeada.

    ```c
    /* mymemory.h */
    typedef struct allocation {
        void *start;
        size_t size;
        struct allocation *next;
    } allocation_t;
    ```

2. **`mymemory_t`**: Representa o pool total de memória e suas alocações. Contém:
   - `pool`: Ponteiro para o início do bloco de memória.
   - `total_size`: Tamanho total do pool de memória.
   - `head`: Ponteiro para a primeira alocação (cabeça da lista encadeada).

    ```c
    /* mymemory.h */
    typedef struct {
        void *pool;
        size_t total_size;
        allocation_t *head;
    } mymemory_t;
    ```

### Funções Principais

- `mymemory_t* mymemory_init(size_t size)`: Aloca o pool de memória e retorna um ponteiro para ele.
- `void* mymemory_alloc(mymemory_t *memory, size_t size)`: Tenta alocar um bloco de tamanho especificado. Retorna um ponteiro para o início do bloco ou NULL em caso de falha.
- `void mymemory_free(mymemory_t *memory, void *ptr)`: Libera a alocação apontada por `ptr`. Ignora o ponteiro se ele não é válido.
- `void mymemory_display(mymemory_t *memory)`: Exibe todas as alocações atuais, mostrando o início e o tamanho de cada bloco.
- `void mymemory_stats(mymemory_t *memory)`: Exibe estatísticas gerais sobre o pool de memória, incluindo:
  - Total de alocações
  - Memória alocada e livre (em bytes)
  - Maior bloco contínuo livre
  - Número de fragmentos de memória livre
- `void mymemory_cleanup(mymemory_t *memory)`: Libera todos os recursos, incluindo as alocações e o pool de memória.

### Estratégias de Alocação

A função `mymemory_alloc` deve implementar uma das seguintes estratégias:
- **Primeiro Ajuste (First Fit)**: Encontra a primeira lacuna suficientemente grande.
- **Melhor Ajuste (Best Fit)**: Encontra a menor lacuna que atenda ao tamanho necessário.
- **Pior Ajuste (Worst Fit)**: Encontra a maior lacuna disponível.

Para verificar o funcionamento do alocador, recomenda-se usar blocos de tamanhos variados (por exemplo, 10, 25, 50, 100, etc.) e testar com diferentes sequências de alocação.

### Restrições

- **Não é permitido** usar funções de alocação global (como `malloc()` e `free()`) exceto para o pool de memória e as estruturas de controle (`mymemory_t` e `allocation_t`).
- O código deve ser funcional, eficiente e sem vazamentos de memória.

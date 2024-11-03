# Trabalho 2 - Programação de Software Básico

## Índice
- [Introdução](#introdução)
- [Especificações](#especificações)
  - [Estruturas de Dados](#estruturas-de-dados)
  - [Funções Principais](#funções-principais)
  - [Estratégias de Alocação](#estratégias-de-alocação)
- [Testes Realizados](#testes-realizados)
  - [Teste 1: Alocações Básicas](#teste-1-alocações-básicas)
  - [Teste 2: Liberação e Realocação](#teste-2-liberação-e-realocação)
  - [Teste 3: Teste de Fragmentação](#teste-3-teste-de-fragmentação)
- [Estatísticas de Memória](#estatísticas-de-memória)
- [Restrições](#restrições)

## Introdução

Este repositório contém o código para o segundo trabalho da disciplina de Programação de Software Básico. O objetivo é desenvolver uma biblioteca simples de gerenciamento de memória em C, que realiza alocações e liberações em um pool de memória predefinido. O gerenciador de memória utiliza o método **Best Fit** para otimizar o uso de memória, minimizando a fragmentação ao encontrar o menor bloco livre que atenda ao tamanho solicitado.

## Especificações

### Estruturas de Dados

1. **`allocation_t`**: Representa um bloco de memória alocado, contendo:
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

1. **`mymemory_t`**: Representa o pool total de memória e suas alocações, contendo:
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
- `void* mymemory_alloc(mymemory_t *memory, size_t size)`: Tenta alocar um bloco de tamanho especificado usando a estratégia **Best Fit**. Retorna um ponteiro para o início do bloco ou NULL em caso de falha.
- `void mymemory_free(mymemory_t *memory, void *ptr)`: Libera a alocação apontada por `ptr`, ignorando o ponteiro se ele não for válido.
- `void mymemory_display(mymemory_t *memory)`: Exibe todas as alocações atuais, mostrando o início e o tamanho de cada bloco.
- `void mymemory_stats(mymemory_t *memory)`: Exibe estatísticas gerais sobre o pool de memória, incluindo:
  - Total de alocações
  - Memória alocada e livre (em bytes)
  - Maior bloco contínuo livre
  - Número de fragmentos de memória livre
- `void mymemory_cleanup(mymemory_t *memory)`: Libera todos os recursos, incluindo as alocações e o pool de memória.

### Estratégias de Alocação

A função `mymemory_alloc` deve implementar a estratégia **Best Fit**, que encontra a menor lacuna que atenda ao tamanho necessário. Para verificar o funcionamento do alocador, recomenda-se usar blocos de tamanhos variados (por exemplo, 10, 25, 50, 100, etc.) e testar com diferentes sequências de alocação.

## Testes Realizados

### Teste 1: Alocações Básicas

#### Estado após alocações iniciais:
- **Tamanho Total do Pool**: 1.048.576 bytes
- **Alocações de Memória**:
  - Bloco 1: offset = 0, tamanho = 100
  - Bloco 2: offset = 100, tamanho = 200
  - Bloco 3: offset = 300, tamanho = 150
- **Espaço Livre Final**: offset = 450, tamanho = 1.048.126

#### Estatísticas de Memória:
- Total de alocações: 3
- Memória total alocada: 450 bytes
- Memória total livre: 1.048.126 bytes
- Maior bloco livre: 1.048.126 bytes
- Número de fragmentos: 1

---

### Teste 2: Liberação e Realocação

#### Liberando bloco de 200 bytes...
- **Tamanho Total do Pool**: 1.048.576 bytes
- **Alocações de Memória**:
  - Bloco 1: offset = 0, tamanho = 100
  - **Espaço Livre**: offset = 100, tamanho = 200
  - Bloco 2: offset = 300, tamanho = 150
- **Espaço Livre Final**: offset = 450, tamanho = 1.048.126

#### Alocando novo bloco de 180 bytes...
- **Tamanho Total do Pool**: 1.048.576 bytes
- **Alocações de Memória**:
  - Bloco 1: offset = 0, tamanho = 100
  - Bloco 2: offset = 100, tamanho = 180
  - **Espaço Livre**: offset = 280, tamanho = 20
  - Bloco 3: offset = 300, tamanho = 150
- **Espaço Livre Final**: offset = 450, tamanho = 1.048.126

#### Estatísticas de Memória:
- Total de alocações: 3
- Memória total alocada: 430 bytes
- Memória total livre: 1.048.146 bytes
- Maior bloco livre: 1.048.126 bytes
- Número de fragmentos: 2

---

### Teste 3: Teste de Fragmentação

#### Alocando sequência de blocos...
- **Tamanho Total do Pool**: 1.048.576 bytes
- **Alocações de Memória**:
  - Bloco 1: offset = 0, tamanho = 100
  - Bloco 2: offset = 100, tamanho = 180
  - **Espaço Livre**: offset = 280, tamanho = 20
  - Bloco 3: offset = 300, tamanho = 150
  - Bloco 4: offset = 450, tamanho = 50
  - Bloco 5: offset = 500, tamanho = 100
  - Bloco 6: offset = 600, tamanho = 150
  - Bloco 7: offset = 750, tamanho = 200
  - Bloco 8: offset = 950, tamanho = 250
- **Espaço Livre Final**: offset = 1.200, tamanho = 1.047.376

#### Liberando blocos alternados para criar fragmentação...
- **Tamanho Total do Pool**: 1.048.576 bytes
- **Alocações de Memória**:
  - Bloco 1: offset = 0, tamanho = 100
  - Bloco 2: offset = 100, tamanho = 180
  - **Espaço Livre**: offset = 280, tamanho = 20
  - Bloco 3: offset = 300, tamanho = 150
  - **Espaço Livre**: offset = 450, tamanho = 50
  - Bloco 4: offset = 500, tamanho = 100
  - **Espaço Livre**: offset = 600, tamanho = 150
  - Bloco 5: offset = 750, tamanho = 200
- **Espaço Livre Final**: offset = 950, tamanho = 1.047.626

#### Estatísticas de Memória:
- Total de alocações: 5
- Memória total alocada: 730 bytes
- Memória total livre: 1.047.846 bytes
- Maior bloco livre: 1.047.626 bytes
- Número de fragmentos: 4

---

## Restrições

- **Não é permitido** usar funções de alocação global (como `malloc()` e `free()`) exceto para o pool de memória e as estruturas de controle (`mymemory_t` e `allocation_t`).
- O código deve ser funcional, eficiente e sem vazamentos de memória.

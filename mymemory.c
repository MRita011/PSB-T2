/* mymemory.c - Implementação do gerenciador de memória
 * Este arquivo implementa um gerenciador de memória simples que utiliza
 * a estratégia de "best fit" para alocação de memória.
 */
#include "mymemory.h"
#include <stdio.h>
#include <stdlib.h>

/* Inicializa o gerenciador de memória
 * Parâmetros:
 *   size: tamanho total do pool de memória a ser gerenciado
 * Retorno:
 *   ponteiro para a estrutura de controle ou NULL em caso de erro
 */
mymemory_t *mymemory_init(size_t size) {
  // Aloca estrutura de controle
  mymemory_t *memory = (mymemory_t *)malloc(sizeof(mymemory_t));
  if (!memory)
    return NULL;

  // Aloca pool de memória do tamanho solicitado
  memory->pool = malloc(size);
  if (!memory->pool) {
    free(memory);
    return NULL;
  }

  // Inicializa os campos da estrutura
  memory->total_size = size;
  memory->head = NULL; // Lista de alocações começa vazia
  return memory;
}

/* Aloca um bloco de memória usando estratégia de best-fit
 * Parâmetros:
 *   memory: ponteiro para o gerenciador de memória
 *   size: tamanho do bloco a ser alocado
 * Retorno:
 *   ponteiro para o bloco alocado ou NULL se não houver espaço
 */
void *mymemory_alloc(mymemory_t *memory, size_t size) {
  if (!memory || !size)
    return NULL;

  // Ponteiros para percorrer a lista de alocações
  allocation_t *curr = memory->head;
  allocation_t *prev = NULL;
  void *start = memory->pool;

  // Variáveis para controlar o melhor ajuste encontrado
  void *best_start = NULL;
  size_t best_size =
      memory->total_size + 1; // Inicializa com valor maior que possível
  allocation_t *best_prev = NULL;

  // Busca o menor espaço livre que comporta o tamanho solicitado
  while (curr || start < (char *)memory->pool + memory->total_size) {
    // Calcula espaço disponível até próxima alocação ou fim do pool
    size_t available =
        curr ? (char *)curr->start - (char *)start
             : (char *)memory->pool + memory->total_size - (char *)start;

    // Atualiza melhor ajuste se encontrar espaço menor que o atual
    if (available >= size && available < best_size) {
      best_size = available;
      best_start = start;
      best_prev = prev;
    }

    if (curr) {
      // Avança para próximo espaço livre após alocação atual
      start = (char *)curr->start + curr->size;
      prev = curr;
      curr = curr->next;
    } else {
      break;
    }
  }

  // Se não encontrou espaço adequado, retorna NULL
  if (!best_start)
    return NULL;

  // Cria e configura novo nó de alocação
  allocation_t *new_node = (allocation_t *)malloc(sizeof(allocation_t));
  if (!new_node)
    return NULL;

  new_node->start = best_start;
  new_node->size = size;

  // Insere o novo nó mantendo a ordem por endereço
  if (!best_prev) {
    // Insere no início da lista
    new_node->next = memory->head;
    memory->head = new_node;
  } else {
    // Insere depois do nó anterior
    new_node->next = best_prev->next;
    best_prev->next = new_node;
  }

  return best_start;
}

/* Libera um bloco de memória previamente alocado
 * Parâmetros:
 *   memory: ponteiro para o gerenciador de memória
 *   ptr: ponteiro para o bloco a ser liberado
 */
void mymemory_free(mymemory_t *memory, void *ptr) {
  if (!memory || !ptr)
    return;

  // Busca o nó de alocação correspondente ao ponteiro
  allocation_t *curr = memory->head;
  allocation_t *prev = NULL;

  while (curr && curr->start != ptr) {
    prev = curr;
    curr = curr->next;
  }

  // Se encontrou o nó, remove da lista e libera
  if (curr) {
    if (prev)
      prev->next = curr->next;
    else
      memory->head = curr->next;
    free(curr);
  }
}

/* Exibe o estado atual das alocações de memória
 * Parâmetros:
 *   memory: ponteiro para o gerenciador de memória
 */
void mymemory_display(mymemory_t *memory) {
  if (!memory) {
    printf("Gerenciador de memória inválido\n");
    return;
  }

  printf("\nAlocações de Memória:\n");
  printf("Tamanho Total do Pool: %zu bytes\n", memory->total_size);

  allocation_t *curr = memory->head;
  void *last_end = memory->pool;
  int bloco = 1;

  // Percorre a lista mostrando blocos alocados e espaços livres
  while (curr) {
    char *curr_start = (char *)curr->start;
    char *pool_start = (char *)memory->pool;
    size_t gap = curr_start - (char *)last_end;

    // Mostra espaço livre antes do bloco atual, se houver
    if (gap > 0) {
      printf("Espaço Livre: offset = %zu tamanho = %zu\n",
             (char *)last_end - pool_start, gap);
    }

    // Mostra informações do bloco alocado
    printf("Bloco %d: offset = %zu tamanho = %zu\n", bloco++,
           curr_start - pool_start, curr->size);

    last_end = curr_start + curr->size;
    curr = curr->next;
  }

  // Mostra espaço livre final, se houver
  size_t final_gap =
      (char *)memory->pool + memory->total_size - (char *)last_end;
  if (final_gap > 0) {
    printf("Espaço Livre Final: offset = %zu tamanho = %zu\n",
           (char *)last_end - (char *)memory->pool, final_gap);
  }
  printf("\n");
}

/* Exibe estatísticas sobre o uso da memória
 * Parâmetros:
 *   memory: ponteiro para o gerenciador de memória
 */
void mymemory_stats(mymemory_t *memory) {
  if (!memory) {
    printf("Gerenciador de memória inválido\n");
    return;
  }

  size_t total_alocado = 0;
  size_t maior_bloco_livre = 0;
  int num_alocacoes = 0;
  int num_fragmentos = 0;
  char *pos_atual = (char *)memory->pool;
  allocation_t *curr = memory->head;

  // Calcula estatísticas percorrendo a lista de alocações
  while (curr) {
    size_t gap = (char *)curr->start - pos_atual;
    if (gap > 0) {
      num_fragmentos++;
      if (gap > maior_bloco_livre)
        maior_bloco_livre = gap;
    }

    total_alocado += curr->size;
    num_alocacoes++;
    pos_atual = (char *)curr->start + curr->size;
    curr = curr->next;
  }

  // Verifica espaço livre final
  size_t gap_final = (char *)memory->pool + memory->total_size - pos_atual;
  if (gap_final > 0) {
    num_fragmentos++;
    if (gap_final > maior_bloco_livre)
      maior_bloco_livre = gap_final;
  }

  // Exibe as estatísticas calculadas
  printf("\nEstatísticas de Memória:\n");
  printf("Total de alocações: %d\n", num_alocacoes);
  printf("Memória total alocada: %zu bytes\n", total_alocado);
  printf("Memória total livre: %zu bytes\n",
         memory->total_size - total_alocado);
  printf("Maior bloco livre: %zu bytes\n", maior_bloco_livre);
  printf("Número de fragmentos: %d\n\n", num_fragmentos);
}

/* Libera todos os recursos do gerenciador de memória
 * Parâmetros:
 *   memory: ponteiro para o gerenciador de memória
 */
void mymemory_cleanup(mymemory_t *memory) {
  if (!memory)
    return;

  // Libera todos os nós da lista de alocações
  allocation_t *curr = memory->head;
  while (curr) {
    allocation_t *next = curr->next;
    free(curr);
    curr = next;
  }

  // Libera o pool de memória e a estrutura de controle
  free(memory->pool);
  free(memory);
}
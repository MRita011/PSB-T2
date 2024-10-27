#include "mymemory.h"
#include <stdio.h>

int main() {
  // Inicializa um pool de memória de 1MB
  mymemory_t *memory = mymemory_init(1024 * 1024);
  if (!memory) {
    printf("Erro ao inicializar o gerenciador de memória\n");
    return 1;
  }

  printf("Teste do Gerenciador de Memória (Best Fit)\n");
  printf("------------------------------------------\n");

  // Teste 1: Alocações básicas
  printf("\nTeste 1: Alocações básicas\n");
  void *bloco1 = mymemory_alloc(memory, 100);
  void *bloco2 = mymemory_alloc(memory, 200);
  void *bloco3 = mymemory_alloc(memory, 150);

  mymemory_display(memory);
  mymemory_stats(memory);

  // Teste 2: Liberação e realocação
  printf("\nTeste 2: Liberação e realocação\n");
  mymemory_free(memory, bloco2); // Libera o bloco do meio
  mymemory_display(memory);

  // Tenta alocar um bloco que se encaixe no espaço liberado
  void *bloco4 = mymemory_alloc(memory, 180);
  printf("\nApós alocar novo bloco:\n");
  mymemory_display(memory);
  mymemory_stats(memory);

  // Teste 3: Fragmentação
  printf("\nTeste 3: Teste de fragmentação\n");
  void *blocos[5];
  for (int i = 0; i < 5; i++) {
    blocos[i] = mymemory_alloc(memory, 50 * (i + 1));
  }
  mymemory_display(memory);

  // Libera blocos alternados para criar fragmentação
  for (int i = 0; i < 5; i += 2) {
    mymemory_free(memory, blocos[i]);
  }

  printf("\nApós liberar blocos alternados:\n");
  mymemory_display(memory);
  mymemory_stats(memory);

  // Limpeza final
  mymemory_cleanup(memory);
  printf("\nTestes concluídos.\n");

  return 0;
}
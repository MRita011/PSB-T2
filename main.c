/* main.c - Programa de teste do gerenciador de memória
   Autores: Maria Rita e Gabriel Kowaleski */
#include "mymemory.h"
#include <stdio.h>

void print_header(const char *title) {
  printf("\n═══════════════════════════════════════════════════════\n");
  printf("           %s", title);
  printf("\n═══════════════════════════════════════════════════════\n");
}

void print_section(const char *section) { 
  printf("\n%s \n", section); 
}

int main() {
  // Inicializa um pool de memória de 1MB
  mymemory_t *memory = mymemory_init(1024 * 1024);
  if (!memory) {
    printf("❌ Erro ao inicializar o gerenciador de memória!\n");
    return 1;
  }

  print_header("GERENCIADOR DE MEMÓRIA (BEST FIT)");

  // Teste 1: Alocações básicas
  print_section("TESTE 1: Alocações Básicas");
  void *bloco1 = mymemory_alloc(memory, 100);
  void *bloco2 = mymemory_alloc(memory, 200);
  void *bloco3 = mymemory_alloc(memory, 150);

  printf("\n→ Estado após alocações iniciais:\n");
  mymemory_display(memory);
  mymemory_stats(memory);

  // Teste 2: Liberação e realocação
  print_section("TESTE 2: Liberação e Realocação");
  printf("\n→ Liberando bloco de 200 bytes...\n");
  mymemory_free(memory, bloco2);
  mymemory_display(memory);

  printf("\n→ Alocando novo bloco de 180 bytes...\n");
  void *bloco4 = mymemory_alloc(memory, 180);
  mymemory_display(memory);
  mymemory_stats(memory);

  // Teste 3: Fragmentação
  print_section("TESTE 3: Teste de Fragmentação");
  printf("\n→ Alocando sequência de blocos...\n");
  void *blocos[5];
  for (int i = 0; i < 5; i++) {
    blocos[i] = mymemory_alloc(memory, 50 * (i + 1));
  }
  mymemory_display(memory);

  printf("\n→ Liberando blocos alternados para criar fragmentação...\n");
  for (int i = 0; i < 5; i += 2) {
    mymemory_free(memory, blocos[i]);
  }
  mymemory_display(memory);
  mymemory_stats(memory);

  // Limpeza final
  print_section("..FINALIZANDO");
  mymemory_cleanup(memory);
  printf("\n✨ Testes concluídos com sucesso!\n");

  return 0;
}
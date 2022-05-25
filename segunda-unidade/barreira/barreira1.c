#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define QTD_THREADS 5

int entrada[QTD_THREADS];
int saida[QTD_THREADS];
int contador;

void* processa_parte(void *p) {
  long id = (long) p;
  while (1) {
    /* Codigo da Terfa */
    printf("[%ld] Iniciando Tarefa...\n", id);
    saida[id] = pow(entrada[id], 2);
    printf("[%ld] Tarefa: %d\n", id, entrada[id]);
    printf("[%ld] Resultado da Tarefa: %d\n", id, saida[id]);
    sleep((int) (rand() % 3));
    printf("[%ld] Embarreirada\n", id);
    /* Fim do Codigo da Terfa */
    /* Ponto de Barreira */
    __sync_fetch_and_add(&contador, 1);
    while((contador % QTD_THREADS) != 0);
  }
  
  
}

int main(int argc, char const *argv[]) {
  
  time_t t;
  pthread_t threads[QTD_THREADS];
  contador = 0;

  srand((unsigned) time(&t));

  for (int i = 0; i < QTD_THREADS; i++) {
    entrada[i] = rand() % 100;
  }

  for (long i = 0; i < QTD_THREADS; i++) {
    pthread_create(&threads[i], NULL, processa_parte, (void*) i);
  }
  
  sleep(30);

  return 0;
}

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define QTD_THREADS 5

int entrada[QTD_THREADS];
int saida[QTD_THREADS];

int arrive[QTD_THREADS];
int proceed[QTD_THREADS];

void* worker(void *p) {
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
    arrive[id] = 1;
    while (proceed[id] == 0)
      usleep(3000); //refresco
    proceed[id] = 0;
  }
}

void* coordinator(void *p) {
  while (1) {
    for (int i = 0; i < QTD_THREADS; i++) {
      while (arrive[i] == 0)
        usleep(3000); //refresco
      arrive[i] = 0;
    }
    printf("[coord] Liberando barreiras...\n");
    for (int i = 0; i < QTD_THREADS; i++) {
      proceed[i] = 1;
    }
  }
}


int main(int argc, char const *argv[]) {
  
  time_t t;
  pthread_t threads[QTD_THREADS];
  pthread_t coord;

  srand((unsigned) time(&t));

  for (int i = 0; i < QTD_THREADS; i++) {
    entrada[i] = rand() % 100;
  }

  for (long i = 0; i < QTD_THREADS; i++) {
    pthread_create(&threads[i], NULL, worker, (void*) i);
  }
  pthread_create(&coord, NULL, coordinator, NULL);
  
  sleep(30);

  return 0;
}

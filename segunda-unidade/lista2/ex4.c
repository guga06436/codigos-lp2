#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define QTD_THREADS 6
#define QTD_PALAVRAS 10

int contador_barreira = 0;

pthread_barrier_t controle_barreira;

// Vetor que armazena strings
char palavras[QTD_PALAVRAS][QTD_THREADS + 1];

void* worker(void *p) {
  long id = (long) p;

  for (int i = 0; i < QTD_PALAVRAS; i++) {
    /* Codigo da Tarefa */
    printf("[%ld] Iniciando Tarefa...\n", id);

    printf("[%ld] iteracao: %d", id, i);
    sleep(1 + id);
    char c = (rand() % 26) + 65;
    palavras[i][id] = c;

    printf("[%ld] Tarefa Finalizada | Embarreirada\n", id);

    int wait_ret = pthread_barrier_wait(&controle_barreira);

    if ((wait_ret != 0) && (wait_ret != PTHREAD_BARRIER_SERIAL_THREAD)) {
      exit(0);
    }

    printf("[%ld] Passou da barreira\n", id);
  }
}


int main(int argc, char const *argv[]) {
  
  time_t t;
  pthread_t threads[QTD_THREADS];

  srand(time(&t));

  int init_bar_ret = pthread_barrier_init(&controle_barreira, NULL, QTD_THREADS);

  if (init_bar_ret != 0) {
    printf("ERRO\n");
    exit(0);
  }

  for (long i = 0; i < QTD_THREADS; i++) {
    pthread_create(&threads[i], NULL, worker, (void*) i);
  }
  
  for (int i = 0; i < QTD_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  for (int i = 0; i < QTD_PALAVRAS; i++) {
    printf("%s\n", palavras[i]);
  }

  return 0;
}

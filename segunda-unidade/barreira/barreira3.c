#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define QTD_THREADS 5

int contador_barreira = 0;

pthread_barrier_t controle_barreira;

void* worker(void *p) {
  long id = (long) p;
  while (1) {
    /* Codigo da Terfa */
    printf("[%ld] Iniciando Tarefa...\n", id);
    
    sleep(1 + id);

    printf("[%ld] Tarefa Finalizada | Embarreirada\n", id);

    int wait_ret = pthread_barrier_wait(&controle_barreira);

    if ((wait_ret != 0) && (wait_ret != PTHREAD_BARRIER_SERIAL_THREAD)) {
      exit(0);
    }

    printf("[%ld] Passou da barreira\n", id);

  }
}


int main(int argc, char const *argv[]) {
  
  pthread_t threads[QTD_THREADS];

  int init_bar_ret = pthread_barrier_init(&controle_barreira, NULL, QTD_THREADS);

  if (init_bar_ret != 0) {
    exit(0);
  }

  for (long i = 0; i < QTD_THREADS; i++) {
    pthread_create(&threads[i], NULL, worker, (void*) i);
  }
  
  sleep(30);

  return 0;
}

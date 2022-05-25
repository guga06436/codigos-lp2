#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <semaphore.h>

#define QTD_THREADS 5

sem_t mutex;
int repeat = 1;
int var_global = 0;

void *cs(void *p) {
  long index = (long) p;
  printf("[%ld] Iniciando...\n", index);
  while(repeat) {
    sem_wait(&mutex);
    var_global++;
    printf("[%ld] Alterou var global: %d\n", index, var_global);
    sem_post(&mutex);
    sleep(1);
  }
  printf("[%ld] Finalizando...\n", index);

}

int main(int argc, char const *argv[]) {
  
  pthread_t threads[QTD_THREADS];
  sem_init(&mutex, 0, 1);

  for (long i = 0; i < QTD_THREADS; i++)
  {
    pthread_create(&threads[i], NULL, cs, (void *) i);
  }

  sleep(10);

  repeat = 0;
  

  return 0;
}

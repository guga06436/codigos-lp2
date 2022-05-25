#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <semaphore.h>

#define QTD_THREADS 5

sem_t arrive1, arrive2;

/*
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
*/

void *worker1(void *p) {
  sleep((rand() % 5) + 1);
  printf("Worker 1 na barreira\n");
  sem_post(&arrive1);
  sem_wait(&arrive2);
  printf("Worker 1 passou da barreira\n");
}

void *worker2(void *p) {
  sleep((rand() % 5) + 1);
  printf("Worker 2 na barreira\n");
  sem_post(&arrive2);
  sem_wait(&arrive1);
  printf("Worker 2 passou da barreira\n");
}

int main(int argc, char const *argv[]) {
  
  time_t t;
  pthread_t w1, w2;

  srand(time(&t));

  sem_init(&arrive1, 0, 0);
  sem_init(&arrive2, 0, 0);

  pthread_create(&w1, NULL, worker1, NULL);
  pthread_create(&w2, NULL, worker2, NULL);

  pthread_join(w1, NULL);
  pthread_join(w2, NULL);
  

  return 0;
}

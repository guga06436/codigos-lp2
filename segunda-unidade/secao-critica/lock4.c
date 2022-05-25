#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define QTD_THREADS 5

int var_global = 0;
int number = 0;
int next = 0;
int turn[QTD_THREADS];


int lock;
int var_global;

void* cs(void* p) {
  while (1) {
    long id = (long) p;
    turn[id] = __sync_fetch_and_add(&number, 1);
    while (turn[id] != next); // protocolo de entrada

    var_global++; // secao critica
    printf("[%ld] valor da var global: %d\n", id, var_global);
    sleep(1);
    next++; // protocolo de saida
  }
  
}

int main(int argc, char const *argv[]) {
  
  pthread_t threads[QTD_THREADS];

  for (long i = 0; i < QTD_THREADS; i++) {
    pthread_create(&threads[i], NULL, cs, (void *) i);
  }

  for (int i = 0; i < QTD_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}

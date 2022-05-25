#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

/* Solução usando o algoritmo de ticket porque
   atende as quatro propriedades da solução da
   seção crítica */

#define QTD_THREADS 5

int var_global = 0;
int number = 0;
int next = 0;
int turn[QTD_THREADS];

int lock;
int clientes[QTD_THREADS];

void* cs(void* p) {
  while (1) {

    long id = (long) p;
    turn[id] = __sync_fetch_and_add(&number, 1);

    // protocolo de entrada
    while (turn[id] != next); 

    // inicio da secao critica
    
    int op = rand() % 2;
    int i = rand() % QTD_THREADS;
    if (op == 0) { //operacao de credito
      clientes[i] += rand() % 1000;
    } else { // operacao de debito
      clientes[i] -= rand() % 1000;
    }
    
    printf("[%ld] Saldo do cliente %d: R$%d\n", id, i, clientes[i]);
    sleep(1);
    // fim da secao critica

    // protocolo de saida
    next++;
  }
  
}

int main(int argc, char const *argv[]) {
  
  time_t t;
  pthread_t threads[QTD_THREADS];

  srand((unsigned) time(&t));

  for (int i = 0; i < QTD_THREADS; i++) {
    clientes[i] = (rand() % 1000) + 1;
  }

  for (long i = 0; i < QTD_THREADS; i++) {
    pthread_create(&threads[i], NULL, cs, (void *) i);
  }

  sleep(10);
  /*
  for (int i = 0; i < QTD_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  */

  return 0;
}

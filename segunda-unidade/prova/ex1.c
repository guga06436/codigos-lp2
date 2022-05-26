#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

/* Solução usando o algoritmo de ticket porque
   atende as quatro propriedades da solução da
   seção crítica */

/* Uma constante para a QTD de Threads em um unico vetor*/
/* Segunda constante para o total de threads */
#define QTD_THREADS 5
#define QTD_THREADS_TOTAL 10

// Cria duas variaveis globais x e y
int x = 0;
int y = 0;

// Para o par
int number = 0;
int next = 0;
int turn[QTD_THREADS];

// Para o impar
int number_impar = 0;
int next_impar = 0;
int turn_impar[QTD_THREADS];

void* cs_par(void* p) {
  
  while (1) {

    long id = (long) p;
    turn[id] = __sync_fetch_and_add(&number, 1);

    // protocolo de entrada
    while (turn[id] != next); 

    // inicio da secao critica
    //incrementa x
    int inc = (rand() % 5) + 1;
    x += inc;
    printf("[%ld] Saldo de X: %d\n", id, x);
    usleep(250);
    // fim da secao critica

    // protocolo de saida
    next++;
  }
  
}

void* cs_impar(void* p) {
  
  while (1) {

    long id = (long) p;
    turn_impar[id] = __sync_fetch_and_add(&number_impar, 1);

    // protocolo de entrada
    while (turn_impar[id] != next_impar); 
    
    // inicio da secao critica
    // incrementa y
    int inc = (rand() % 24) + 2;
    y += inc;
    printf("[%ld] Saldo de Y: %d\n", id, y);
    usleep(300);
    // fim da secao critica

    // protocolo de saida
    next_impar++;
  }
  
}



int main(int argc, char const *argv[]) {
  
  time_t t;
  pthread_t threads[QTD_THREADS_TOTAL];

  srand((unsigned) time(&t));

  for (long i = 0; i < QTD_THREADS_TOTAL; i++) {
    if (i % 2 == 0) {
        pthread_create(&threads[i], NULL, cs_par, (void *) i);
    } else {
        pthread_create(&threads[i], NULL, cs_impar, (void *) i);
    }
  }

  sleep(10);

  return 0;
}

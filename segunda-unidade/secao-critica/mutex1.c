#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

//spinlock

#define QTD_THREADS 5

int lock = 0;
int var_global = 0;
pthread_mutex_t mutex_lock_control = PTHREAD_MUTEX_INITIALIZER;

void *cs(void *p) {

  long idx = (long) p;
  while (1) {

    // protocolo de entrada
    pthread_mutex_lock(&mutex_lock_control);

    // inicio da secao critica
    var_global++;
    printf("[CS-%ld] Alterou o valor para: %d\n", idx, var_global);
    //sleep(1);
    // fim da seção critica

    // protocolo de saida
    pthread_mutex_unlock(&mutex_lock_control);
    
  }
}

int main(int argc, char const *argv[]) {
  
  pthread_t threads[5];

  for (long i = 0; i < QTD_THREADS; i++) {
    pthread_create(&threads[i], NULL, cs, (void *) i);
  }

  sleep(20);

  return 0;
}

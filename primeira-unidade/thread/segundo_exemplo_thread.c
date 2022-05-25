#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 5

long var_global;

void *funcao_thread(void *param) {

  long id = (long) param;

  printf("[#%ld] Thread %ld criada\n", id, id);
  sleep(5);

  var_global++;
  printf("[#%ld] varGlobal = %ld\n", id, var_global);
  pthread_exit((void*) var_global + id);

}


int main(int argc, char const *argv[]) {
  
  pthread_t threads[QTD_THREADS];
  int thread_return[QTD_THREADS];

  var_global = 0;

  printf("Dentro da funcao main\n");
  printf("Vou criar %d threads\n", QTD_THREADS);

  for (long i = 0; i < QTD_THREADS; i++) {
    pthread_create(&threads[i], NULL, funcao_thread, (void*) i);
  }

  for (int i = 0; i < QTD_THREADS; i++) {
    printf("Main agurdando thread %d\n", i);
    pthread_join(threads[i], (void *) &thread_return[i]);
    printf("thread %d retornou %d\n", i, thread_return[i]);
  }

  printf("Dentro da função MAIN\nvarGlobal = %ld\n", var_global);
  
  return 0;
}

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <semaphore.h>

#define QTD_THREADS 5

sem_t arrive[QTD_THREADS];

void *worker(void *p) {

  long id = (long) p;

  sleep((rand() % 5) + 1);
  printf("Worker %ld na barreira\n", id);
  sem_post(&arrive[id]);
  for (int i = 0; i < QTD_THREADS; i++) {
    if (i != id) {
      int sem_v;
      sem_getvalue(&arrive[i], &sem_v);
      if(sem_v != 0) {
        sem_wait(&arrive[id]);
      }
    }
  }
  
  printf("Worker %ld passou da barreira\n", id);
}

int main(int argc, char const *argv[]) {
  
  time_t t;
  pthread_t threads[QTD_THREADS];

  srand(time(&t));

  for (int i = 0; i < QTD_THREADS; i++) {
    sem_init(&arrive[i], 0, 0);
  }

  for (long i = 0; i < QTD_THREADS; i++)
  {
    pthread_create(&threads[i], NULL, worker, (void *) i);
  }

  for (int i = 0; i < QTD_THREADS; i++)
  {
    pthread_join(threads[i], NULL);
  }
  

  return 0;
}

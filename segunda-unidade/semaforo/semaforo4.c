#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <semaphore.h>

#define QTD_PROD 1
#define QTD_CONS 3

int buf;
sem_t empty, full;

void *producer(void *p) {
  long id = (long) p;
  int data;
  printf("[P%ld] Iniciando...\n", id);
  while (1) {
    data = rand() % 255;
    sem_wait(&empty);
    buf = data;
    printf("[P%ld] Gerou: %d\n", id, data);
    sem_post(&full);
  }
  
}

void *consumer(void *p) {
  long id = (long) p;
  int result;
  printf("[C%ld] Iniciando...\n", id);
  while (1) {
    sem_wait(&full);
    result = buf;
    printf("[C%ld] Consumiu: %d\n", id, result);
    sem_post(&empty);
  }
}

int main(int argc, char const *argv[])
{

  time_t t;
  pthread_t p[QTD_PROD], c[QTD_CONS];

  sem_init(&empty, 0, 1);
  sem_init(&full, 0, 0);

  srand(time(&t));

  for (long i = 0; i < QTD_PROD; i++) {
    pthread_create(&p[i], NULL, producer, (void *) i);
  }

  for (long i = 0; i < QTD_CONS; i++) {
    pthread_create(&c[i], NULL, consumer, (void *) i);
  }
  
  sleep(20);

  return 0;
}

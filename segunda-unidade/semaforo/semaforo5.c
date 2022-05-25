#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <semaphore.h>

#define QTD_PROD 2
#define QTD_CONS 4
#define TAM_BUFFER 20

int buf[TAM_BUFFER];
sem_t empty, full;
sem_t mutexDep, mutexFetch;
int front = 0, rear = 0;

void *producer(void *p) {
  long id = (long) p;
  int data;
  printf("[P%ld] Iniciando...\n", id);
  while (1) {
    data = rand() % 255;
    sem_wait(&empty);
    sem_wait(&mutexDep);
    buf[rear] = data;
    rear = (rear + 1) % TAM_BUFFER;
    printf("[P%ld] Gerou: %d\n", id, data);
    sleep(1);
    sem_post(&mutexDep);
    sem_post(&full);
  }
  
}

void *consumer(void *p) {
  long id = (long) p;
  int result;
  printf("[C%ld] Iniciando...\n", id);
  while (1) {
    sem_wait(&full);
    sem_wait(&mutexFetch);
    result = buf[front];
    front = (front + 1) % TAM_BUFFER;
    printf("[C%ld] Consumiu: %d\n", id, result);
    sem_post(&mutexFetch);
    sem_post(&empty);
  }
}

int main(int argc, char const *argv[])
{

  time_t t;
  pthread_t p[QTD_PROD], c[QTD_CONS];

  sem_init(&empty, 0, TAM_BUFFER);
  sem_init(&full, 0, 0);

  sem_init(&mutexDep, 0, 1);
  sem_init(&mutexFetch, 0, 1);

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

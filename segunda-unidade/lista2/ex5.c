/*5) Faça um programa em C/C++ onde são criadas 2 threads: uma thread produtora, e a outra consumidora. A thread produtora deverá gerar 100 valores inteiros aleatórios entre 0 e 255 e armazenar em um array compartilhado com 10 posições. A thread consumidora buscará as informações no array e exibi-los, de forma que nenhum valor seja perdido. Use semáforos implementando a solução do buffer compartilhado. */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <semaphore.h>

#define TAM_BUFFER 10

int buf[TAM_BUFFER];
sem_t empty, full;
sem_t mutexDep, mutexFetch;
int front = 0, rear = 0;

void *producer(void *p) {
  int data;
  printf("[P] Iniciando...\n");
  for (int i = 0; i < 100; i++) {
    /* code */
    data = rand() % 256;
    sem_wait(&empty);
    sem_wait(&mutexDep);
    buf[rear] = data;
    rear = (rear + 1) % TAM_BUFFER;
    printf("[P-%d] Gerou: %d\n", i, data);
    sem_post(&mutexDep);
    sem_post(&full);
  }
  
}

void *consumer(void *p) {
  int result;
  printf("[C] Iniciando...\n");
  for (int i = 0; i < 100; i++) {
    sem_wait(&full);
    sem_wait(&mutexFetch);
    result = buf[front];
    front = (front + 1) % TAM_BUFFER;
    printf("[C-%d] Consumiu: %d\n", i, result);
    sem_post(&mutexFetch);
    sem_post(&empty);
  }
}


int main(int argc, char const *argv[])
{

  time_t t;
  pthread_t p, c;

  sem_init(&empty, 0, TAM_BUFFER);
  sem_init(&full, 0, 0);

  sem_init(&mutexDep, 0, 1);
  sem_init(&mutexFetch, 0, 1);

  srand(time(&t));

  
  pthread_create(&p, NULL, producer, NULL);
  pthread_create(&c, NULL, consumer, NULL);

  pthread_join(p, NULL);
  pthread_join(c, NULL);

  return 0;
}

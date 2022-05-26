#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <semaphore.h>

// tamanho do buffer de 1024 chars
#define TAM_BUFFER 1024

// buffer
int buf[TAM_BUFFER];

// variaveis desemaforos
sem_t empty, full;
sem_t mutexDep, mutexFetch;
int front = 0, rear = 0;

// funcao producer
void *producer(void *p) {
  int data;
  printf("[P] Iniciando...\n");
  for (int volta = 0; volta < 4; volta++) {
      for (int i = 0; i < TAM_BUFFER; i++) {
        // gera char aleatorio como int
        data = (rand() % 26) + 65;
        // semaforo segura ate o consumidor agir
        sem_wait(&empty);
        sem_wait(&mutexDep);
        // operacao com o buffer
        buf[rear] = data;
        rear = (rear + 1) % TAM_BUFFER;
        // imprime e dorme
        printf("[P-%d] Gerou: %c\n", i, data);
        usleep(50);
        //semaforo libera
        sem_post(&mutexDep);
        sem_post(&full);
      }
  }
}

// funcao consumer
void *consumer(void *p) {
  int result;
  // inicia arquivo
  FILE *arq = fopen("ArqGrav.txt", "wt");
  
  printf("[C] Iniciando...\n");
  for (int volta = 0; volta < 4; volta++) {
      for (int i = 0; i < TAM_BUFFER; i++) {
        //espera produtor
        sem_wait(&full);
        sem_wait(&mutexFetch);
        // consome buffer
        result = buf[front];
        front = (front + 1) % TAM_BUFFER;
        // imprime, grava e dorme
        printf("[C-%d] Consumiu: %c\n", i, result);
        fprintf(arq,"[C-%d] Consumiu: %c\n", i, result);
        usleep((rand() % 60) + 30);
        //semaforo libera
        sem_post(&mutexFetch);
        sem_post(&empty);
      }
  }
  //fecha arquivo
  fclose(arq);
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

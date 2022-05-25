#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define QTD_THREADS 5

typedef struct pedaco_imaem {
  int index;
  char type;
  int data_chunk;
} PedacoImagem;

void *processa_estrutura(void *param) {

  PedacoImagem *meu_pedaco = (PedacoImagem *) param;
  long pot;

  printf("[%d] type: %c\n", meu_pedaco->index, meu_pedaco->type);
  printf("[%d] data_chunk: %d\n", meu_pedaco->index, meu_pedaco->data_chunk);
  printf("[%d] processando...\n", meu_pedaco->index);

  sleep(rand() % 10 + meu_pedaco->index);
  pot = pow((double) meu_pedaco->data_chunk, (double) 2);

  printf("[%d] data_chunk processado = %ld\n", meu_pedaco->index, pot);
  pthread_exit((int *) pot);
}

int main(int argc, char const *argv[]) {
  
  time_t t;
  PedacoImagem imagem[QTD_THREADS];
  pthread_t threads[QTD_THREADS];
  long resultados[QTD_THREADS];

  srand((unsigned) time(&t));

  for (int i = 0; i < QTD_THREADS; i++) {
    imagem[i].index = i + 1;
    imagem[i].type = 65 + i;
    imagem[i].data_chunk = rand() % 255;
  }

  for (int i = 0; i < QTD_THREADS; i++) {
    pthread_create(&threads[i], NULL, processa_estrutura, (void *) &imagem[i]);
  }
  
  for (int i = 0; i < QTD_THREADS; i++) {
    pthread_join(threads[i], (void *) &resultados[i]);
    printf("[main] Resultado recebido da thread %d: %ld\n",
          i + 1, resultados[i]);
  }
  

  return 0;
}

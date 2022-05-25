/*
Montanha Russa concorrente
Suponha que existam n passageiros e um carro em uma montanha russa em um parque. Os passageiros, repetidamente, esperam para dar uma volta no carro. O carro tem capacidade para C passageiros, com C < n.  O carro só pode partir  quando estiver cheio. Após dar uma volta na montanha russa, cada passageiro passeia pelo parque de diversões e depois retorna à montanha russa para a próxima volta. O programa a ser desenvolvido deverá utilizar o algoritmo do ticket apresentado em sala de aula. 

Tanto o carro quanto os passageiros devem ser representados por threads. A implementação das threads dos passageiros e dos carros devem executar funções que se baseiam nos seguintes pseudocódigos:

thread passageiro[i = 1 to n]  {
    while (!fechouParque) { 
        entraNoCarro();

  // Incrementa contador que registra o número de passageiros transportados pelo carro.//
        esperaVoltaAcabar(); 
        saiDoCarro(); // decrement o contador
        passeiaPeloParque(); // tempo aleatório
    }
}

thread carro {
    while (existemPassageirosNoParque) {
        esperaEncher();
        daUmaVolta();
        esperaEsvaziar(); 
        volta++; // Indicador para o fechamento do parque.
    }
}

Locks devem ser utilizados para a sincronização do acesso às seções críticas, barreiras devem sincronizar as múltiplas threads. A implementação deverá ser feita em C/C++ utilizando a biblioteca Pthreads ou as threads STD (C++11). A implementação deverá atender às quatro propriedades de uma solução para o problema da seção crítica: exclusão mútua, ausência de deadlock, ausência de atraso desnecessário e entrada eventual. A saída do seu programa deve ser bem planejada, de forma a mostrar o que está acontecendo a cada momento. 

*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define QTD_PESSOAS 10
#define CARRO_MAX 6

pthread_barrier_t controle_barreira;

int qtd_carro;

void* worker(void *p) {
  /*
  long id = (long) p;

  for (int i = 0; i < QTD_PALAVRAS; i++) {
    // Codigo da Tarefa 
    printf("[%ld] Iniciando Tarefa...\n", id);

    printf("[%ld] iteracao: %d", id, i);
    sleep(1 + id);
    char c = (rand() % 26) + 65;
    palavras[i][id] = c;

    printf("[%ld] Tarefa Finalizada | Embarreirada\n", id);

    int wait_ret = pthread_barrier_wait(&controle_barreira);

    if ((wait_ret != 0) && (wait_ret != PTHREAD_BARRIER_SERIAL_THREAD)) {
      exit(0);
    }

    printf("[%ld] Passou da barreira\n", id);
  }
  */
}


int main(int argc, char const *argv[]) {

/*  
  time_t t;
  pthread_t threads[QTD_THREADS];

  srand(time(&t));

  int init_bar_ret = pthread_barrier_init(&controle_barreira, NULL, QTD_THREADS);

  if (init_bar_ret != 0) {
    printf("ERRO\n");
    exit(0);
  }

  for (long i = 0; i < QTD_THREADS; i++) {
    pthread_create(&threads[i], NULL, worker, (void*) i);
  }
  
  for (int i = 0; i < QTD_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  for (int i = 0; i < QTD_PALAVRAS; i++) {
    printf("%s\n", palavras[i]);
  }
*/
  return 0;

}

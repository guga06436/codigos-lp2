/*
1) Faça um programa em C/C++ em que você compare o tempo de criação de 30 
threads e 30 processos, onde as threads e processos criadas apenas executem
uma função que exiba uma mensagem ("Nova thread" ou "Novo processo") e as faça
dormir por 2s. Use alguma função de medição de tempo* para tal e escreva suas
impressões sobre o resultado, respondendo qual das opções demandou menos tempo
de execução para ser realizada e faça a relação disso com consumo de 
processamento e memória. Para finalizar, faça uma clara distinção (em 
comentários no código) das diferenças principais entre threads e processos.
 Ao término do programa, nenhum processo deverá permanecer em execução.

* Funções sugeridas para uso no processo de medição de tempo de execução são 
listadas abaixo. 
- Em C/C++: time(),  clock(), clock_gettime() (biblioteca time.h), 
  gettimeofday() (biblioteca sys/time.h);
- Em C++: método chrono::high_resolution_clock (biblioteca chrono). 
- Sugestão de referência: https://tinyurl.com/medirtempoemc
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>

#define NUMERO_PROCESSOS 30
#define QTD_THREADS 30

void * funcao_thread(void *param) {
  sleep(2);
}


int main(int argc, char const *argv[]) {

  pid_t pid_original;
  clock_t t_p0, t_p1, t_p;
  clock_t t_t0, t_t1, t_t;
  pthread_t threads[QTD_THREADS];

  pid_original = getpid();

  t_p0 = clock();

  for (int i = 0; i < NUMERO_PROCESSOS; i++) {
    if (pid_original == getpid()) {
      fork();
    }
  }
  
  if (getpid() != pid_original) {
    sleep(2);
  } else {
    
    t_p1 = clock();
    t_p = t_p1 - t_p0;
    printf("A criação de processos demorou  %lf ms\n", ((double)t_p)/(CLOCKS_PER_SEC/1000));

    t_t0 = clock();

    for (int i = 0; i < QTD_THREADS; i++) {
      pthread_create(&threads[i], NULL, funcao_thread, NULL);
    }
  
    t_t1 = clock();
    t_t = t_t1 - t_t0;
    printf("A criação de threads demorou  %lf ms\n", ((double)t_t)/(CLOCKS_PER_SEC/1000));
  }

  return 0;
}

//RESULTADO: 0.001670 seg
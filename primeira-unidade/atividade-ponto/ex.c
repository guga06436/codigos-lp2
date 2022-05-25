#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

/* Prototipos */
void *funcao_thread(void* p);
void corrida(long id, int d_max);

/* Variaveis Globais */
int pista;
int n_coelhos;

int main(int argc, char const *argv[]) {
  
  /* Caso o usuario erre as entradas ele finaliza execucao */
  if (argc != 4) {
    printf("1\n");
    return(-1);
  }
  /* Executa por processo */
  else if (strcmp(argv[1], "-p") == 0) {
    
    /* Declara um array de pids contendo o tamanho equivalente ao
    numero de coelhos informados, alem do tamanho da pista, o id do
    coelho vencedor e o pid original */
    n_coelhos = atoi(argv[2]);
    pid_t pid_original, pid_clones[n_coelhos];
    int retorno_ganhador;
    pista = atoi(argv[3]) * 100;

    // Armazena o pid original
    pid_original = getpid();
    
    // Laço de repetição para a geração de processos
    for (int i = 0; i < n_coelhos; i++) {
      
      // Se for o processo original ele clona
      if (getpid() == pid_original) {
        pid_clones[i] = fork();
      }
      // Caso seja o processo filho, chama a função de corrida e retorna
      // o indice do coelho para caso tenha ganho
      if (getpid() != pid_original) {
        srand(time(NULL) + i*i);
        int d = (rand() % 31) + 30;
        corrida(i, d);
        return i;
      }
    }

    // Espera por algum processo filho terminar, mata o restante
    // e imprime o ganhador 
    if (getpid() == pid_original) {
      wait(&retorno_ganhador);
      for (int i = 0; i < n_coelhos; i++) {
        kill(pid_clones[i], SIGKILL);
      }
      printf("Lebre %d venceu!\n", WEXITSTATUS(retorno_ganhador));
    }

  }
  /* Executa por thread*/
  else if (strcmp(argv[1], "-t") == 0) {

    /* Declara um array de threads contendo o tamanho equivalente ao
       numero de coelhos informados, alem do tamanho da pista */
    n_coelhos = atoi(argv[2]);
    pthread_t threads[n_coelhos];
    pista = atoi(argv[3]) * 100;

    srand(time(NULL));

    /* Cria as threads */
    for (long i = 0; i < n_coelhos; i++) {
      pthread_create(&threads[i], NULL, funcao_thread, (void *) i);
    }
    
    /* Espera as threads acabarem */
    for (int i = 0; i < n_coelhos; i++) {
      pthread_join(threads[i], NULL);
    }
  }

  /* Entrada do usuario nao condiz */
  else {
    return -1;
  }
  
  return 0;
}

void *funcao_thread(void* p) {

  /* Gera randomicamente a distancia maxima do salto e
     pega indice da lebre */
  int d = (rand() % 31) + 30;
  long id = (long) p;

  /* Aplica funcao da corrida */
  corrida(id, d);

  // Imprime lebre ganhadora
  printf("Lebre %ld venceu!\n", id);

  // Encerra o programa e cancela as outras threads
  exit(1);

}

void corrida(long id, int d_max) {
  
  int total = 0;
  int salto;

  for (int i = 0; total < pista; i++) {

    // Geracao aleatoria do salto, incremento no total 
    salto = (rand() % (d_max - 9)) + 10;
    total += salto;

    // Tentativa de diminuir saidas caso tenha acabado
    if (total < pista - 1) {
      sleep(1);
    }

    // Saida
    printf("lebre %ld saltou %dcm (total: %d)\n", id, salto, total);
  }

}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

#define QTD_THREADS 10
#define QTD_INCREMENTOS 50
#define QTD_DECREMENTOS 30

double var_global;

void *funcao_thread(void *param) {
    
    long id = (long) param;
    printf("%ld\n", id);
    
    time_t t;

    srand(time(&t));

    // se numro eh par incrementa var global 50 vezes
    if (id % 2 == 0) {
        for (int i = 0; i < QTD_INCREMENTOS; i++) {
            var_global++;
            printf("[%ld] var_global = %f\n", id, var_global);
        }
        sleep((rand() % 4) + 1);
    } 
    // se numro eh impar decrementa var global 30 vezes
    else {
        for (int i = 0; i < QTD_DECREMENTOS; i++) {
            var_global--;
            printf("[%ld] var_global = %f\n", id, var_global);
        }
        //sleep(2);
    }
    
}

int main(int argc, char const *argv[]) {
    
    time_t t;
    pid_t pid_original, return_fork;
    pthread_t threads[10];
    int resultado;
    
    pid_original = getpid();

    srand(time(&t));
    
    return_fork = fork();

    // Faz o random no filho e espera ele acabar
    if (getpid() != pid_original) {
        return rand() % 100;
    } else {
        waitpid(return_fork, &resultado, 0);
        printf("Resultado do random foi: %d\n", WEXITSTATUS(resultado));
    }

    var_global = 0;

    // cria threads
    for (long i = 0; i < QTD_THREADS; i++) {
        pthread_create(&threads[i], NULL, funcao_thread, (void*) i);
    }

    for (int i = 0; i < QTD_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("O valor da varíavel global foi de: %f\n", var_global);

    return 0;
}

/*

Concorrência é um paradigma da programação no qual os programas fazem uso da execução simultânea
de várias tarefas computacionais. Divida em entrelaçamento (programas progridem juntos, mas
nao usam o mesmo processador) e paralelismo (processos progridem e executam juntos)

Dentre os elementos do SO e Hardware destacam-se a memória ram, memoria principal e o processador
que atuam no gerenciamento de threads e processos. Também há o escalonador.

*/
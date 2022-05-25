/*
(Questão 1 - 5 pontos) 

Faça um programa em C/C++ que crie um arquivo de texto 
que funcionará como um dicionário contendo todas as strings com 4 caracteres 
alfabéticos maiúsculos (de "AAAA" a "ZZZZ"). Cada letra do alfabeto (de A a Z)
deverá ter sua parte do dicionário gerada por um processo ou thread individual,
(o programa deverá receber um parâmetro na sua execução que determine se threads
ou processos serão usados) que gerará todas as strings iniciadas por uma letra 
(ex. processo/thread que gerará as strings começando com a letra 'C' gerará as 
strings de "CAAA" a "CZZZ"). A thread principal do processo original deverá 
aguardar a geração de todas as partes e gerar um arquivo de texto contendo o 
dicionário inteiro. Comente no código as diferenças do uso de processos e 
threads, do ponto de vista do consumo de recursos e do ponto de vista do uso
das funções/API.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define QTD_ARQUIVOS 24
#define FILE_NAME "saida.txt"
#define MAX_FILE_NAME 7

typedef struct file_search {
  char filename[MAX_FILE_NAME];
} FileSearch;


void *funcao_thread(void *param);

int main(int argc, char const *argv[]) {
    
    /* CASO argv[1] seja thread ele executa por threads */
    if (strcmp(argv[1], "thread") == 0) {
        
        pthread_t threads[QTD_ARQUIVOS];
        FileSearch files[QTD_ARQUIVOS];
        long resultados[QTD_ARQUIVOS];
        char filename[QTD_ARQUIVOS];
        
        //CRIA THREADS
        for (char i = 65; i <= 90; i++) {
            pthread_create(&threads[(int) i - 65], NULL, funcao_thread, (void *) &i);
        }

        //ESPERA THREADS
        for (int i = 0; i < QTD_ARQUIVOS; i++) {
            pthread_join(threads[i], (void *) &resultados[i]);
        }

        return 0;
    }

    /* CASO argv[1] seja PROCESSO ele executa por PROCESSO */
    else if (strcmp(argv[1], "processo") == 0) {

        pid_t pid_original;
        char c0 = 65;

        pid_original = getpid();

        for (int i = 0; i < QTD_ARQUIVOS; i++) {
            
            if (pid_original == getpid()) {
                fork();
            }

            if (pid_original != getpid()) {
                for (char i = 65; i <= 90; i++) {
                    for (char k = 65; k <= 90; k++) {
                        for (char j = 65; j <= 90; j++) {
                            printf("%c%c%c%c\n", c0, i, k, j);
                        }
                    }
                }
                return 0;
            }
            c0++;
        }

        return 0;
    }

    else {
        printf("ERRO\n");
        return (-1);
    }
    
}

void *funcao_thread(void *param) {
    
    // Tive problema tentando converter o param para char
    char c0 = (char) param;

    // Printa string
    for (char i = 65; i <= 90; i++) {
        for (char k = 65; k <= 90; k++) {
            for (char j = 65; j <= 90; j++) {
                printf("%c%c%c%c\n", c0, i, k, j);
            }
        }
    }
}

/*
threads: um processo em execução que possui multiplos fluxos de controle
processo: um programa em execução juntamente com o contexto referente a ele

threads são mais eficientes por não exigir os mecanismos de comunicação dos processos,
justamente por compartilharem o mesmo espaço de endereçamento. Em relação às funções, threads precisam
da biblioteca pthread.h e processos pode ser feito por meio da unistd.h
*/
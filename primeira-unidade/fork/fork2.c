#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  
  int qntd_processos;
  pid_t pid_original;
  int fork_return;

  pid_original = getpid();

  printf("Mensagem antes do fork() - pid original: %d\n", pid_original);
  printf("Digite a quantidade de processos a existir: ");
  scanf("%d", &qntd_processos);

  if (qntd_processos > 1) {
    
    fork_return = fork();

    if (fork_return > 0) {
      for (int i = 0; i < qntd_processos - 2; i++) {
        if (fork_return > 0) {
          fork_return = fork();
        }
      }
    }
  }

  printf("Mesnagem depois do fork() - Processo PID %d\n", getpid());

  return 0;
}

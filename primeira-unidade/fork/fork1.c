#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  
  char c;

  printf("Programa que se clonara\n");
  printf("Aperte qualquer tecla para clonar essa execução: ");
  scanf("%c", &c);

  fork();

  printf("Mensagem apos o fork\n");
  printf("Dormirei por 20s...\n");
  sleep(20);
  printf("Finalizando as duas instancias!\n");

  return 0; 
}

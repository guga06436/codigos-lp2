#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int varGlobal;

void *funcao_thread(void *param) {
  
  printf("Dentro da função thread\nvarGlobal = %d\n", varGlobal);
  sleep(10);
  varGlobal++;
  printf("Saindo da função thread\nvarGlobal = %d\n", varGlobal);

}

int main(int argc, char const *argv[]) {
  
  varGlobal = 1000;
  pthread_t thread1, thread2;

  printf("Dentro da funcao main\n");
  printf("Vou criar duas threads\n");

  pthread_create(&thread1, NULL, funcao_thread, NULL);
  pthread_create(&thread2, NULL, funcao_thread, NULL);

  sleep(15);

  printf("Dentro da função MAIN\nvarGlobal = %d\n", varGlobal);

  return 0;
}

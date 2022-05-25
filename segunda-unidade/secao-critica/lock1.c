#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

int in1, in2;
int var_global;

void* cs1(void* p) {
  while (1) {
    // < await (!in2) in1 = 1; > // p entrada
    while (in2 == 1);
    in1 = 1;
    printf("cs1 na secao critica\n");
    var_global++;
    sleep(3);
    printf("[cs1] valor da var global: %d\n", var_global);
    in1 = 0; // p saida
    printf("cs1 fora da secao critica\n");
    sleep(1);
  }
  
}

void* cs2(void* p) {
    // < await (!in1) in2 = 1; > // p entrada
    while (in1 == 1);
    in2 = 1;
    printf("cs2 na secao critica\n");
    var_global++;
    sleep(3);
    printf("[cs2] valor da var global: %d\n", var_global);
    in2 = 0; // p saida
    printf("cs2 fora da secao critica\n");
    sleep(1);
}

int main(int argc, char const *argv[]) {
  
  pthread_t thread1, thread2;
  var_global = 0;
  in1, in2 = 0;

  pthread_create(&thread1, NULL, cs1, NULL);
  pthread_create(&thread2, NULL, cs2, NULL);

  sleep(20);

  return 0;
}

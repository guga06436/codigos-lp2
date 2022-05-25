#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

int lock;
int var_global;

void* cs(void* p) {
  while (1) {
    // < await (!lock) lock = 1; > // p entrada
    while (__sync_lock_test_and_set(&lock, 1));
    lock = 1;
    printf("cs1 na secao critica\n");
    var_global++;
    sleep(3);
    printf("[cs1] valor da var global: %d\n", var_global);
    lock = 0; // p saida
    printf("cs1 fora da secao critica\n");
    sleep(1);
  }
  
}

int main(int argc, char const *argv[]) {
  
  pthread_t thread1, thread2;
  var_global = 0;
  lock = 0;

  pthread_create(&thread1, NULL, cs, NULL);
  pthread_create(&thread2, NULL, cs, NULL);

  sleep(20);

  return 0;
}

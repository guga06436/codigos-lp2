#include <stdio.h>

int main(int argc, char const *argv[]) {
  
  int valor;

  scanf("%d", &valor);
  printf("%d", valor);

  for (int i = 1; i <= valor; i++) {
    printf("Linha %d de Prog1\n", i);
  }

  return 0;
}

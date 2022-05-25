#include <stdio.h>

int main(int argc, char const *argv[]) {
  
  int valor;
  char* linha = "Prog2: ";
  char texto[99];

  scanf("%d", &valor);

  for (int i = 0; i < valor; i++) {
    scanf(" %99[^\n]", texto);
    printf("%s %s\n", linha, texto);
  }
  printf("\n");

  return 0;
}

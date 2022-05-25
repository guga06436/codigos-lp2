/*

2) Faça dois programas em C que solicitem uma string para o usuário (com até 8 
caracteres). Os dois programas irão contar as ocorrências da string em um dos
6 arquivos anexados à esta lista utilizando concorrência - um deles usará 
threads e o outro processos. Cada arquivo deverá ser processado por uma 
thread/processo separado e no fim o programa deverá apresentar a soma das 
ocorrências calculadas por cada processo antes de seu término. Comente o 
código com as diferença do uso de threads e processos (do ponto de vista do
programador). 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define NUMERO_PROCESSOS 6

int Search_in_File(char *fname, char *str);

int main(int argc, char const *argv[]) {

  pid_t pid_original, pid_clones[NUMERO_PROCESSOS];
  char texto[9];
  char filename[6];

  int ret_clones[NUMERO_PROCESSOS];
  int count = 0;

  int status;
  
  scanf(" %8[^\n]s", texto);

  
  pid_original = getpid();
  for (int i = 0; i < NUMERO_PROCESSOS; i++) {
    if (getpid() == pid_original) {
      pid_clones[i] = fork();
    }

    if (getpid() != pid_original) {
      sprintf(filename, "%d", i);
      strcat(filename, ".txt");
      return Search_in_File(filename, texto);
    }
  }

  if (pid_original == getpid()) {
    for (int i = 0; i < NUMERO_PROCESSOS; i++) {
      waitpid(pid_clones[i], &ret_clones[i], 0);
      count += WEXITSTATUS(ret_clones[i]);
    }
    printf("A palavra se repetiu %d vezes\n", count);
  }

  return 0;
}


int Search_in_File(char *fname, char *str) {
	
  FILE *fp;
	int line_num = 1;
	int find_result = 0;
	char temp[512];
	
	//gcc users
	if((fp = fopen(fname, "r")) == NULL) {
		return(-1);
	}

	while(fgets(temp, 512, fp) != NULL) {
		if((strstr(temp, str)) != NULL) {
			printf("A match found on line %d of file %s\n", line_num, fname);
			find_result++;
		}
		line_num++;
	}

	if(find_result == 0) {
		printf("Sorry, couldn't find a match in file %s.\n", fname);
	}
	
	//Close the file if still open.
	if(fp) {
		fclose(fp);
	}
  return find_result;
}
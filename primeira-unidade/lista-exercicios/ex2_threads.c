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
#include <pthread.h>

#define NUMERO_THREADS 6

typedef struct file_search {
  char filename[6];
  char text[9];
} FileSearch;


void *funcao_thread(void *p);
long Search_in_File(char *fname, char *str);

int main(int argc, char const *argv[]) {
  
  pthread_t threads[NUMERO_THREADS];
  FileSearch files[NUMERO_THREADS];
  long resultados[NUMERO_THREADS];

  int count = 0;

  char texto[9];
  scanf(" %8[^\n]s", texto);

  for (int i = 0; i < NUMERO_THREADS; i++) {
    
    sprintf(files[i].filename, "%d", i);
    strcat(files[i].filename, ".txt");
    strcpy(files[i].text, texto);

    pthread_create(&threads[i], NULL, funcao_thread, (void *) &files[i]);

  }

  for (int i = 0; i < NUMERO_THREADS; i++) {
    pthread_join(threads[i], (void *) &resultados[i]);
    count += resultados[i];
  }

  printf("Houve %d encontros\n", count);
  

  return 0;
}


void *funcao_thread(void *p) {
  
  FileSearch *meu_pedaco = (FileSearch *) p;

  long result = Search_in_File(meu_pedaco->filename, meu_pedaco->text);

  pthread_exit((long *) result);
}


long Search_in_File(char *fname, char *str) {
	
  FILE *fp;
	int line_num = 1;
	long find_result = 0;
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
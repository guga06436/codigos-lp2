
/*
---------- !!!OBSERVACAO!!! ----------
Por alguma razao com o telnet nao funciona, tem que usar nc
---------- !!!OBSERVACAO!!! ----------
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>

#define QTD_CLIENTS 8 // QTD DE CLIENTES SUPORTADOS
#define PORT 9999 // PORTA DO SERVIDOR
#define BUFF_SIZE 8 // TAMANHO DO BUFFER

// Estrutura do monitor para tratamento do arquivo
typedef struct monitor {
	FILE *arq;
	sem_t mutex;
} t_monitor;
t_monitor monitor_arq;

// Funcao para escrever no arquivo
void mon_write(char* s) {
	sem_wait(&monitor_arq.mutex); //protocolo entrada
	fprintf(monitor_arq.arq, "%s\n", s);
	sem_post(&monitor_arq.mutex); //protocolo saida
}

// Contador de clientes
int clients = 0;

// funcao tratamento do cliente
void* handle_client(void* p) {

    // socket do cliente
	long cli_sock = (long) p;

    // envia mensagem de welcome
    char *server_welcome = "WELCOME\n";
    write(cli_sock, server_welcome, strlen(server_welcome));

    // inicializa buffer
	char buffer[BUFF_SIZE+1];

	while(1) {

        // le comando do cliente
		read(cli_sock, buffer, BUFF_SIZE);
		buffer[BUFF_SIZE] = '\0';

        // escreve mensagem por meio de uma funcao
        mon_write(buffer);

        /* CASO 1: Envia um numero aleatorio entre 0 e 1000
           CASO 2: Envia data e hora atual
           CASO 3: Finaliza conexao com cliente
           CASO 4: Derruba servidor
           CASO 5: COMANDO NAO RECONHECIDO
        */
		if (strcmp("RNDNUM\n", buffer) == 0) {
			int op = rand() % 1001;
            char str_return[BUFF_SIZE];
		    sprintf(str_return, "%d\n", op);
            write(cli_sock, str_return, strlen(str_return));
            printf("Cliente %ld recebeu numero aleatorio\n", cli_sock);
		} else if (strcmp("CRTIME\n", buffer) == 0) {

            char day[3], month[3], year[5];
            int hour;

            time_t now = time(NULL);
            struct tm *t = localtime(&now);

            sprintf(day, "%02d", t->tm_mday, sizeof(day));
            sprintf(month, "%02d", t->tm_mon + 1, sizeof(month));
            sprintf(year, "%04d", t->tm_year + 1900, sizeof(year));
            hour = t->tm_hour;

            char msg[30];
            sprintf(msg, "%dh  of  %s / %s / %s\n", hour, day, month, year);
            write(cli_sock, msg, strlen(msg));
            printf("Cliente %ld recebeu data/hora\n", cli_sock);

        } else if (strcmp("FINISH\n", buffer) == 0) {
            printf("Cliente %ld encerrou conexao", cli_sock);
            write(cli_sock, "CONEXAO FINALIZADA\n", strlen("CONEXAO FINALIZADA\n"));
            close(cli_sock);
            break;
        } else if (strcmp("SHTDWN\n", buffer) == 0) {
            printf("DERRUBANDO SERVIDOR\n");
            write(cli_sock, "SERVER FINALIZADA\n", strlen("SERVER FINALIZADO\n"));
            fclose(monitor_arq.arq); // fecha log
            exit(0);
        } else {
            char* msg = "COMANDO NAO RECONHECIDO\n";
            write(cli_sock, msg, strlen(msg));
            printf("Comando do cliente %ld nao reconhecido\n", cli_sock);
        }
	}
}

int main(void) {

	int server_sock; // socket do server

  time_t t; // variavel de tempo

	long cli_sock[QTD_CLIENTS]; // array com sockets dos clientes
	pthread_t cli_threads[QTD_CLIENTS]; // array das threads que lidam com os clientes

	struct sockaddr_in server_addr; // informacoes do server
	struct sockaddr_in cli_addr[QTD_CLIENTS]; // informacoes do cliente
	int cli_addr_size[QTD_CLIENTS];

	sem_init(&monitor_arq.mutex, 0, 1); // inicializa monitor
	monitor_arq.arq = fopen("log.txt", "wt"); // abre arquivo
	srand((unsigned) time(&t));

	printf("Iniciando servidor na porta %d...\n", PORT);

    // inicializa socket tcp
	server_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (server_sock < 0) {
		perror("socket() error!\n");
		exit(-1);
	}

    // preenche informacoes
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

    // reebe retorno do bind
	int bind_result = bind(server_sock,
			       (struct sockaddr*) &server_addr,
		               sizeof(server_addr));

	if (bind_result < 0) {
		perror("bind() error!\n");
		exit(-1);
	}

	int listen_result = listen(server_sock, QTD_CLIENTS);

	if (listen_result < 0) {
		perror("listen() error!\n");
		exit(-1);
	}

    // tenta conexao
	while(cli_sock[clients] = accept(server_sock,
				     (struct sockaddr*) &cli_addr[clients],
				      &cli_addr_size[clients])) {

		if (cli_sock < 0 ) {
			perror("accept() error!\n");
			exit(-1);
		}

		printf("Novo cliente conectado!\n");

		char *cli_ip_addr = inet_ntoa(cli_addr[clients].sin_addr);
		printf("Endereco do cliente: %s\n", cli_ip_addr);

		pthread_create(&cli_threads[clients], NULL, handle_client,
				(void*) cli_sock[clients]);

		clients++;
        printf("qtd clients: %d\n", clients);

	}

  fclose(monitor_arq.arq);

	return 0;
}

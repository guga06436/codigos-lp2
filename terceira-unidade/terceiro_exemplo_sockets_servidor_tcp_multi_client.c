#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

#define TAM_MSG 50
#define QTD_CLIENTS 2

int client_counter = 0;

void * handle_client(void *p) {
  
  int my_client_fd = (int) p;

  char *server_welcome = "BEM VINDO\n";
  write(my_client_fd, server_welcome, strlen(server_welcome));

  while (1) {
    char buffer[TAM_MSG];
    char msg[TAM_MSG];
    int read_size;
    read_size = recv(my_client_fd, buffer, TAM_MSG, 0);
    
    if (read_size > 0) {
      printf("Client: %s\n", buffer);
      if (strcmp("exit", msg) == 0) {
        close(my_client_fd);
        client_counter--;
        exit(0);
      }
      
    }
    sprintf(msg, "Contador clientes: %d\n", client_counter);
    write(my_client_fd, msg, strlen(msg));
    
  }


}

int main(int argc, char const *argv[]) {
  
  int sock_fd, client_fd[QTD_CLIENTS];
  unsigned int server_port = 0;
  int client_info_size[QTD_CLIENTS];
  struct sockaddr_in server_info;
  struct sockaddr_in client_info[QTD_CLIENTS];
  pthread_t client_thread[QTD_CLIENTS];

  printf("Digite a porta do servidor: ");
  scanf("%d", &server_port);

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd == -1) {
    printf("Erro socket\n");
    exit(0);
  } else {

    bzero(&server_info, sizeof(server_info));
    for (int i = 0; i < QTD_CLIENTS; i++) {
      bzero(&client_info[i], sizeof(client_info[i]));
    }

    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(server_port);
    server_info.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock_fd, &server_info, sizeof(server_info)) != 0) {
      printf("Erro bind\n");
      exit(0);
    } else {
      printf("Aguardando conexao na porta %d\n", server_port);

      if (listen(sock_fd, QTD_CLIENTS) != 0) {
        printf("Erro listen\n");
        exit(0);
      } else {
        printf("...\n");
        while (client_fd[client_counter] = accept(sock_fd, 
                                            &client_info[client_counter],
                                            &client_info_size[client_counter]))
        {

          char *client_ip_addr = inet_ntoa(client_info[client_counter].sin_addr);
          printf("[%d] Recebi conexao de %s\n", client_counter, client_ip_addr);
          
          pthread_create(&client_thread[client_counter], NULL, handle_client, (void *) client_fd[client_counter]);

          client_counter++;

          printf("Total de %d clientes conectados\n", client_counter);
        }
      }
    }
  }
  return 0;
}
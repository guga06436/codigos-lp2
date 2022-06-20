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

#define TAM_MSG 100
#define TAM_HOST_SERV 100

int main(int argc, char const *argv[]) {
  
  int sock_fd, client_fd;
  unsigned int server_port = 0;
  int client_info_size;
  struct sockaddr_in server_info;
  struct sockaddr_in client_info;

  printf("Digite a porta do servidor: ");
  scanf("%d", &server_port);

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd == -1) {
    printf("Erro socket\n");
    exit(0);
  } else {
    bzero(&server_info, sizeof(server_info));
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(server_port);
    server_info.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock_fd, &server_info, sizeof(server_info)) != 0) {
      printf("Erro bind\n");
      exit(0);
    } else {
      printf("Aguardando conexao na porta %d\n", server_port);

      if (listen(sock_fd, 5) != 0) {
        printf("Erro listen\n");
        exit(0);
      } else {
        printf("...\n");
        client_fd = accept(sock_fd, &client_info, &client_info_size);
        if (client_fd < 0) {
          printf("\nErro ao invocar o accept()\n");
          exit(0);
        } else {
          
          printf("Cliente se conectou!\n");
          char *client_ip_addr = inet_ntoa(client_info.sin_addr);
          printf("Endereco: %s\n", client_ip_addr);
          
          char *server_welcome = "BEM VINDO\n";
          write(client_fd, server_welcome, strlen(server_welcome));

          while (1) {
            char buffer[TAM_MSG];
            char msg[TAM_MSG];
            int read_size;
            read_size = recv(client_fd, buffer, TAM_MSG, 0);
            if (read_size > 0) {
              printf("Client: %s\n", buffer);
            }
            printf("Digite uma mensagem para enviar para o cliente: ");
            scanf(" %101[^\n]", msg);

            write(client_fd, msg, strlen(msg));
            
          }
        } 
      } 
    }
  }
  


  return 0;
}

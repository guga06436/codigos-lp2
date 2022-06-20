#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

#define TAM_MSG_RECV 100
#define TAM_HOST_SERV 100

int main(int argc, char const *argv[]) {
  
  int meu_socket_stream;
  int connection_result;
  int porta_servidor;
  char host_servidor[TAM_HOST_SERV];
  struct sockaddr_in endereco;
  struct sockaddr_in endereco_recv;
  char msg_recv[TAM_MSG_RECV];

  printf("Informe o endereco do server: ");
  scanf(" %101[^\n]", host_servidor);

  printf("Informe a porta: ");
  scanf("%d", &porta_servidor);

  meu_socket_stream = socket(PF_INET, SOCK_STREAM, 0); //TCP

  bzero(&endereco, sizeof(endereco));

  endereco.sin_family = AF_INET;
  endereco.sin_port = htons(porta_servidor);
  inet_aton(host_servidor, &endereco.sin_addr);

  connection_result = connect(meu_socket_stream, &endereco, sizeof(endereco));
  if (!connection_result) {
    printf("Conexao estabelecida com o server %s na porta %d\n", host_servidor, porta_servidor);
  } else {
    printf("Erro de conexao\n");
    exit(0);
  }

  while (1) {

    char* msg_to_send = (char *) malloc(TAM_MSG_RECV * sizeof(char) + 1);
    printf("Digite uma mensagem: ");
    scanf(" %101[^\n]", msg_to_send);

    if (strcmp(msg_to_send, "exit") == 0) {
      close(meu_socket_stream);
      printf("Saindo...\n");
      free(msg_to_send);
      break;
    }

    sendto(meu_socket_stream, (void *) msg_to_send, strlen(msg_to_send), 0, &endereco, sizeof(endereco));
    recvfrom(meu_socket_stream, &msg_recv, TAM_MSG_RECV, 0, &endereco_recv, sizeof(endereco_recv));
    printf("Mensagem do servidor: %s\n", msg_recv);
    free(msg_to_send);
  }
  

  return 0;
}

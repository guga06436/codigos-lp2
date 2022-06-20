#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

#define BUFF_SIZE 256

int myport = 0;
int running = 1;

int main(int argc, char const *argv[]) {

  int socket_fd, resp_socket_fd;
  char buffer[BUFF_SIZE];
  struct sockaddr_in srv_addr, resp_addr, client_addr;
  int addr_len;
  int read_len;
  int client_port;
  char day[3], month[3], year[5];

  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  sprintf(day, "%02d", t->tm_mday, sizeof(day));
  sprintf(month, "%02d", t->tm_mon + 1, sizeof(month));
  sprintf(year, "%04d", t->tm_year + 1900, sizeof(year));
  
  printf("Digite a porta para ouvir UDP: ");
  scanf("%d", &myport);
  client_port = myport + 1;

  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

  if (socket_fd < 0) {
    printf("Erro socket\n");
    exit(0);
  }
  
  bzero(&srv_addr, sizeof(struct sockaddr_in));
  srv_addr.sin_family = AF_INET;
  srv_addr.sin_addr.s_addr = INADDR_ANY;
  srv_addr.sin_port = htons(myport);

  if (bind(socket_fd, &srv_addr, sizeof(srv_addr)) < 0) {
    printf("Erro bind\n");
    exit(0);
  }

  addr_len = sizeof(client_addr);

  while (running) {

    char msg[30];
    read_len = recvfrom(socket_fd, buffer, BUFF_SIZE, NULL, &client_addr, &addr_len);
  
    buffer[read_len + 1] = "\0";
    printf("Recebi do cliente: %s\n", buffer);

    if (strncmp("exit", buffer, 4) == 0) {
      printf("Saindo do server...");
      running = 0;
    }

    sprintf(msg, "%s / %s / %s", day, month, year);

    resp_addr.sin_family = AF_INET;
    resp_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    resp_addr.sin_port = htons(client_port);

    resp_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    sendto(resp_socket_fd, msg, strlen(msg), 0, &resp_addr, sizeof(resp_addr));
    
  }
  


  return 0;
}

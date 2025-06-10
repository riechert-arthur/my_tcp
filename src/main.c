#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "io.h"

int main() {

  while(1) {
    printf("Initializing connection...\n");
    sleep(1);

    int socket = create_tcp_socket();
    if (socket == -1) {
      printf("Failed to create socket!\n");
      continue;
    }

    printf("Socket created successfully!\n");

    int r = listen_on_tcp_socket(socket);
    if (r == -1) {
      printf("Failed to connect to server!\n");
      close(socket);
      continue;
    }

    if (tcb_table[0]->state != ESTAB) {
      printf("No connection established!\n");
      continue;
    }

    uint8_t* data = read_tcp_data(socket, buf);
    if (data == NULL) {
      perror("Error reading data from socket!\n");
      continue;
    }

    printf("Received message: %s\n", data);
    
  }
  
  close(socket);

  return 0;
}

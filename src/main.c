#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "io.h"

int main() {
  uint8_t buffer[MAX_BUFFER_SIZE];

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

    read_from_socket(socket, buffer, sizeof(buffer));
    printf("Received message: %s\n", buffer);
  }
  
  close(socket);

  return 0;
}

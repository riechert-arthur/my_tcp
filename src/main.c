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

    struct addrinfo* addr_info = (struct addrinfo*) getaddrinfo("127.0.0.1", "8080", NULL, NULL);
    if (addr_info == NULL) {
      printf("Failed to get address info!\n");
      close(socket);
      continue;
    }

    struct sockaddr_in* addr = connect_to_server(socket, "127.0.0.1", 8080);
    if (!addr || addr == -1) {
      printf("Failed to connect to server!\n");
      close(socket);
      continue;
    }
    printf("Connected to server!\n");

    read_from_socket(socket, buffer, sizeof(buffer));
    printf("Received message: %s\n", buffer);
  }
  
  close(socket);

  return 0;
}

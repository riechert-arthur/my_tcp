#include "io.h"

/**
 * By default, sockets will use a TCP protocol.
 * We create a raw one because we want to implement
 * a custom protocol. 
 */
int create_tcp_socket() {
  int s = socket(AF_INET, SOCK_DGRAM, 0);
  
  if (s < 0)
    perror("Error creating socket:");
  
  return s;
}

/*
 * Takes the socket file descriptor and binds it to a specific
 * address. For example, we would bind 127.0.0.1:8080 to the
 * socket's file descriptor.
 */ 
int bind_to_tcp_socket(int sock, struct sockaddr_in* saddr, int addrlen) {
  int b = bind(sock, (struct sockaddr*) saddr, addrlen);
  
  if (b < 0)
    perror("Error binding socket to address:");
    
  return b;
}

/*
 * Marks the socket associated with the sock file descriptor
 * as passive, so it can accept incoming connection requests.
 */
int listen_on_tcp_socket(int sock, int backlog) {
  int l = listen(sock, backlog);

  if (l < 0)
    perror("Error setting socket to listen:");
}

int accept_tcp_request(int sock, )

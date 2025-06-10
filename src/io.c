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
 * socket's file descriptor. The address is used with a routing table
 * to direct the data to the local network interface / gateway.
 */ 
int bind_to_tcp_socket(int sock, struct sockaddr_in* saddr, int addrlen) {
  int b = bind(sock, (struct sockaddr*) saddr, addrlen);
  
  if (b < 0)
    perror("Error binding socket to address:");
    
  return b;
}

int listen_on_tcp_socket(int sock) {

  // Default to 1 TCB for now...
  tcb_table->state = LISTEN;
  
  while(1) {
    int r = recv(sock, buf, MAX_BUFFER_SIZE, 0);

    if r < 0 {
      perror("Error listening to socket on address:");
    } else if (r == 0) {
      return 0; 
    }
    
    struct iphdr *ip_header = parse_ip_header(buf);
    tcp_header_t *tcp_header = parse_tcp_header(buf);

    if ((tcb_table->state == LISTEN || tcb_table->state == SYN_SENT) && TCP_SYN(tcp_header->flags_and_offset)) {
      tcb_table->state = SYN_RECVD;
       
      // Send the acknowledge
      
      continue;
    } else if ((tcb_table->state == SYN_SENT || tcb_table->state == SYN_RECVD) && TCP_ACK(tcp_header->flags_and_offset) {
      tcb_table->state = ESTAB;

      break; 
    } 
  }  
}

int send_ack(int socket, uint8_t* buf) {

  uint8_t* ack;
  size_t n = sizeof(struct iphdr) + sizeof(tcp_header_t)

  if (!(ack = (uint8_t*) malloc(n))) {
    errno = "Failed to malloc!" 
    return -1;
  }

  memcpy(ack, buf, n);

  struct iphdr *ip_header = parse_ip_header(ack); 
  tcp_header_t *tcp_header = parse_tcp_header(ack); 

  // Reverse the addresses
  struct in_addr temp = ip_header->ip_src;
  ip_header->ip_src = ip_header->ip_dst;
  ip_header->ip_dst = temp;

  uint16_t temp = tcp_header->source;
  tcp_header->source = tcp_header->destination;
  tcp_header->destination = temp;
  tcp_header->ack_number = tcp->sequence_number + 1;
  
  // Randomly generate a new sequence number
  tcp_header->sequence_number = rand(); 

  tcp_header->flags_and_offset = tcp_make_flags_and_offset(0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0);

  ssize_t s = send(socket, ack, n, 0);

  return 0;
}

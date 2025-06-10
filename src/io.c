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
 * 
 * @param sock The socket to bind.
 * @param saddr The address to bind to.
 * @param addrlen The length of the address.
 * @return The result of the bind operation.
 */ 
int bind_to_tcp_socket(int sock, struct sockaddr_in* saddr, int addrlen) {
  int b = bind(sock, (struct sockaddr*) saddr, addrlen);
  
  if (b < 0)
    perror("Error binding socket to address:");
    
  return b;
}

/*
 * Listens on the socket for incoming connections.
 * 
 * @param sock The socket to listen on.
 * @return The result of the bind operation.
*/
int listen_on_tcp_socket(int sock) {

  // Default to 1 TCB for now...
  if (!tcb_table[0]) {
    tcb_table[0] = (tcb_t*)malloc(sizeof(tcb_t));
  }
  tcb_table[0]->state = LISTEN;
  tcb_table[0]->snd_una = 0;
  tcb_table[0]->snd_nxt = 0;
  tcb_table[0]->seg_ack = 0;
  tcb_table[0]->seg_seq = 0;
  tcb_table[0]->seq_len = 0;
  tcb_table[0]->last_seq = 0;
  
  while(1) {
    int r = recv(sock, rcv_buf, MAX_BUFFER_SIZE, 0);

    if (r < 0) {
      perror("Error listening to socket on address:");
    } else if (r == 0) {
      return 0; 
    }
    
    struct iphdr *ip_header = parse_ip_header(rcv_buf);
    tcp_header_t *tcp_header = parse_tcp_header(rcv_buf);

    if (TCP_SYN(tcp_header->flags_and_offset)) {
      tcb_table[0]->state = SYN_SENT;
      tcb_table[0]->seg_seq = tcp_header->sequence_number;
      tcb_table[0]->seq_len = ip_header->tot_len - ip_header->ihl * 4;
      tcb_table[0]->last_seq = tcp_header->sequence_number;
    }

    if ((tcb_table[0]->state == LISTEN || tcb_table[0]->state == SYN_SENT) && TCP_SYN(tcp_header->flags_and_offset)) {
      tcb_table[0]->state = SYN_RCVD;
       
      // Send the acknowledge
      send_ack(sock, rcv_buf); 

      continue;
    } else if ((tcb_table[0]->state == SYN_SENT 
      || tcb_table[0]->state == SYN_RCVD) 
      && TCP_ACK(tcp_header->flags_and_offset)
      && tcb_table[0]->snd_una < tcb_table[0]->seg_ack
      && tcb_table[0]->seg_ack <= tcb_table[0]->snd_nxt) {

      tcb_table[0]->state = ESTAB;

      break; 
    } 
  }  
}

/*
 * Sends an acknowledge to the client.
 * 
 * @param socket The socket to send the acknowledge to.
 * @param buf The buffer to send the acknowledge from.
 * @return The result of the send operation.
 */
int send_ack(int socket, uint8_t* buf) {

  uint8_t* ack;
  size_t n = sizeof(struct iphdr) + sizeof(tcp_header_t);

  if (!(ack = (uint8_t*) malloc(n))) {
    errno = ENOMEM;
    return -1;
  }

  memcpy(ack, buf, n);

  struct iphdr *ip_header = parse_ip_header(ack); 
  tcp_header_t *tcp_header = parse_tcp_header(ack); 

  // Reverse the addresses
  uint32_t temp = ip_header->saddr;
  ip_header->saddr = ip_header->daddr;
  ip_header->daddr = temp;

  uint16_t temp = tcp_header->source;
  tcp_header->source = tcp_header->destination;
  tcp_header->destination = temp;
  tcp_header->ack_number = tcp_header->sequence_number + 1;
  
  // Randomly generate a new sequence number
  tcp_header->sequence_number = rand(); 

  tcp_header->flags_and_offset = tcp_make_flags_and_offset(0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0);
  tcp_header->checksum = compute_checksum(ip_header, tcp_header);

  ssize_t s = send(socket, ack, n, 0);

  return 0;
}

/*
 * Reads data from the socket.
 * 
 * @param socket The socket to read data from.
 * @param buf The buffer to read data into.
 * @return The data read from the socket.
 */
uint8_t* read_tcp_data(int socket, uint8_t* buf) {
  int r = recv(socket, buf, MAX_BUFFER_SIZE, 0);

  if (r < 0) {
    perror("Error reading data from socket:");
    return NULL;
  }

  struct iphdr* ip_header = parse_ip_header(buf);
  tcp_header_t* tcp_header = parse_tcp_header(buf);

  uint16_t checksum = compute_checksum(ip_header, tcp_header);

  if (checksum != tcp_header->checksum) {
    errno = EINVAL;
    return NULL;
  }

  uint8_t* data = strip_tcp_header(buf);

  if (data == NULL) {
    errno = EINVAL;
    return NULL;
  }
  
  return data;
}
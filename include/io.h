#ifndef IO_H
#define IO_H

#include <sys/socket.h>
#include "tcp_header.h"
#include "tcb.h"
#include <netinet/ip.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define MAX_TCBS 1024
#define MAX_BUFFER_SIZE 4096


int create_tcp_socket();
int bind_to_tcp_socket(int sock, struct sockaddr_in* saddr, int addrlen);
int listen_on_tcp_socket(int sock);
int send_ack(int socket, uint8_t* buf);
uint8_t* read_tcp_data(int socket, uint8_t* buf);

/*
 * The tcb_table is a table of tcb_t structs for controlling
 * the state of each tcp connection.
 */
static tcb_t* tcb_table[MAX_TCBS];


/*
 * The snd_buf is a queue of bytes for sending data.
 */
static uint8_t snd_buf[MAX_BUFFER_SIZE];


/*
 * The rcv_buf is a queue of bytes for reading data.
 */
static uint8_t rcv_buf[MAX_BUFFER_SIZE];


/*
 * Parses the ip header from the buffer.
 * 
 * @param buf The buffer to parse the ip header from.
 * @return The parsed ip header.
 */
static struct iphdr* parse_ip_header(uint8_t *buf) {
  struct iphdr *header = (struct iphdr*) buf;
  return header;
}

/*
 * Parses the tcp header from the buffer.
 * 
 * @param buf The buffer to parse the tcp header from.
 * @return The parsed tcp header.
 */
static tcp_header_t* parse_tcp_header(uint8_t *buf) {
  struct tcp_header_t *header = (tcp_header_t*) (buf + sizeof(struct iphdr)); 
  return header;
}

/*
 * Computes the checksum of the ip and tcp headers.
 * 
 * @param ip_header The ip header to compute the checksum for.
 * @param tcp_header The tcp header to compute the checksum for.
 * @return The computed checksum.
 */
static uint16_t compute_checksum(struct iphdr* ip_header, tcp_header_t* tcp_header) {
  uint32_t left_mask = 0xFFFF << 16;
  uint32_t right_mask = 0xFFFF;

  uint32_t sums = (ip_header->saddr & left_mask) + (ip_header->saddr & right_mask >> 16)
    + (ip_header->daddr & left_mask) + (ip_header->daddr & right_mask >> 16)
    + (ip_header->protocol) + (ip_header->tot_len) + (ip_header->ihl) + (tcp_header->source) + (tcp_header->destination);

  uint16_t checksum;

  if (sums > 0xFFFF - 1) {
    checksum = (sums % (0xFFFF - 1)) + ((sums & left_mask) >> 16); 
  }

  return ~checksum; 
}

/*
 * Returns the offset of the tcp header in the buffer.
 * 
 * @param buf The buffer to strip the ip header from.
 * @return The offset of the tcp header in the buffer.
 */
static struct iphdr* strip_ip_header(uint8_t* buf) {
  return (uint8_t*) buf + sizeof(struct iphdr);
}

/*
 * Returns the offset of the data in the buffer.
 * 
 * @param buf The buffer to strip the tcp header from.
 * @return The offset of the data in the buffer.
 */
static uint8_t* strip_tcp_header(uint8_t* buf) {
  return (uint8_t*) buf + sizeof(struct iphdr) + sizeof(tcp_header_t);
}

#endif

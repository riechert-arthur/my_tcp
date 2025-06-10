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

static tcb_t* tcb_table[MAX_TCBS];
static uint8_t buf[MAX_BUFFER_SIZE];

// Doesn't take into account ip options
static struct iphdr* parse_ip_header(uint8_t *buf) {
  struct iphdr *header = (struct iphdr*) buf;
  return header;
}

// Doesn't take into account ip options
static tcp_header_t* parse_tcp_header(uint8_t *buf) {
  struct tcp_header_t *header = (tcp_header_t*) (buf + sizeof(struct iphdr)); 
  return header;
}

static uint16_t compute_checksum(struct iphdr* ip_header, tcp_header_t* tcp_header) {
  uint32_t left_mask = 0xFFFF << 16;
  uint32_t right_mask = 0xFFFF;

  uint32_t sums = (ip_header->saddr & left_mask) + (ip_header->saddr & right_mask >> 16)
    + (ip_header->daddr & left_mask) + (ip_header->daddr & right_mask >> 16)
    + (ip_header->protocol) + (tcp_header->source) + (tcp_header->destination);

  uint16_t checksum;

  if (sums > 0xFFFF - 1) {
    checksum = (sums % (0xFFFF - 1)) + ((sums & left_mask) >> 16); 
  }

  return ~checksum; 
}

/*
 * Returns the offset of the tcp header in the buffer.
 */
static struct iphdr* strip_ip_header(uint8_t* buf) {
  return (uint8_t*) buf + sizeof(struct iphdr);
}

/*
 * Returns the offset of the data in the buffer.
 */
static uint8_t* strip_tcp_header(uint8_t* buf) {
  return (uint8_t*) buf + sizeof(struct iphdr) + sizeof(tcp_header_t);
}

#endif

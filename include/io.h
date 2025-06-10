#ifndef IO_H
#define IO_H

#include <sys/socket.h>
#include "tcp_header.h"
#include <netinet/ip.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_TCBS 1024
#define MAX_BUFFER_SIZE 4096

static tcb_t* tcb_table[MAX_TCBS];
static uint8_t buf[MAX_BUFFER_SIZE];

// Doesn't take into account ip options
static iphdr* parse_ip_header(uint8_t *buf) {
  struct iphdr *header = (struct iphdr*) buf;
  return header;
}

// Doesn't take into account ip options
static tcp_header_t* parse_tcp_header(uint8_t *buf) {
  struct tcp_header_t *header = (tcp_header_t*) (buf + sizeof(struct iphdr)); 
  return header;
}

#endif

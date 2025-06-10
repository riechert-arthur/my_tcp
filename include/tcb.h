#ifndef TCB_H
#define TCB_H

#include <stdint.h>
#include "io.h"

/*
 * The tcb_state_t enum represents the state of a tcp connection.
 */
typedef enum {
  LISTEN,
  CLOSED,
  SYN_SENT,
  SYN_RCVD,
  ESTAB,
  FIN_WAIT_1,
  FIN_WAIT_2,
  CLOSING,
  CLOSE_WAIT,
  LAST_ACK,
  TIME_WAIT
} tcb_state_t;

/*
 * The tcb_t struct represents a transport control block for a tcp connection.
 * 
 * @param state The state of the connection.
 * @param snd_una The sequence number of the next byte to send.
 * @param snd_nxt The sequence number of the next byte to send.
 * @param seg_ack The sequence number of the next byte to receive.
 * @param seg_seq The sequence number of the next byte to send.
 * @param seq_len The length of the sequence number.
 * @param last_seq The last sequence number received.
 * @param local_port The local port.
 * @param local_address The local address.
 * @param remote_port The remote port.
 * @param remote_address The remote address.
 */
typedef struct {
  tcb_state_t state;
  uint16_t snd_una;
  uint16_t snd_nxt;
  uint16_t seg_ack;
  uint16_t seg_seq;
  uint16_t seq_len;
  uint16_t last_seq;
  uint16_t local_port;
  uint16_t local_address;
  uint16_t remote_port;
  uint16_t remote_address;
  uint16_t ip_security_level;
  uint8_t *rcv_buf;
  uint8_t *snd_buf;
} tcb_t;

/*
 * Initializes a tcb_t struct.
 * 
 * @param local_port The local port.
 * @param local_address The local address.
 * @param remote_port The remote port.
 * @param remote_address The remote address.
 * @return The tcb_t struct.
 */
static tcb_t* create_tcb(
  uint16_t local_port,
  uint16_t local_address,
  uint16_t remote_port,
  uint16_t remote_address
) {

  tcb_t* tcb;

  if(!(tcb = (tcb_t*)malloc(sizeof(tcb_t)))) {
    perror("Error allocating memory for tcb_t struct:");
    return NULL;
  }

  tcb->state = LISTEN;
  tcb->snd_una = 0;
  tcb->snd_nxt = 0;
  tcb->seg_ack = 0;
  tcb->seg_seq = 0;
  tcb->seq_len = 0;
  tcb->last_seq = 0;
  tcb->local_port = local_port;
  tcb->local_address = local_address;
  tcb->remote_port = remote_port;
  tcb->remote_address = remote_address;
  tcb->ip_security_level = 0;
  tcb->rcv_buf = rcv_buf;
  tcb->snd_buf = snd_buf;

  return tcb;
}


#endif

#ifndef TCB_H
#define TCB_H

#include <stdint.h>

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

#endif

#ifndef TCB_H
#define TCB_H

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

typedef struct {
  tcb_state_t state;
} tcb_t;

#endif

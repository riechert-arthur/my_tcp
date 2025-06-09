#ifndef TCP_HEADER_H
#define TCP_HEADER_H

#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint16_t source;        
    uint16_t destination;  

    uint32_t sequence_number; 
    uint32_t ack_number;    

    // 16 bits: DataOffset (4) | Reserved (3) | NS (1) | CWR (1) | ECE (1)
    //          | URG (1) | ACK (1) | PSH (1) | RST (1) | SYN (1) | FIN (1)
    uint16_t flags_and_offset;

    uint16_t window;     
    uint16_t checksum;     
    uint16_t urgent_ptr;  

} tcp_header_t;

#define TCP_OFFSET(f)   (((f) >> 12) & 0xF)
#define TCP_RESERVED(f) (((f) >> 9) & 0x7)
#define TCP_NS(f)       (((f) >> 8) & 0x1)
#define TCP_CWR(f)      (((f) >> 7) & 0x1)
#define TCP_ECE(f)      (((f) >> 6) & 0x1)
#define TCP_URG(f)      (((f) >> 5) & 0x1)
#define TCP_ACK(f)      (((f) >> 4) & 0x1)
#define TCP_PSH(f)      (((f) >> 3) & 0x1)
#define TCP_RST(f)      (((f) >> 2) & 0x1)
#define TCP_SYN(f)      (((f) >> 1) & 0x1)
#define TCP_FIN(f)      ((f) & 0x1)

static inline uint16_t tcp_make_flags_and_offset(
  uint8_t offset, uint8_t reserved, bool ns,
  bool cwr, bool ece, bool urg, bool ack, bool psh,
  bool rst, bool syn, bool fin
) {
  return ((uint16_t)(offset   & 0xF) << 12)
       | ((uint16_t)(reserved & 0x7) << 9)
       | ((uint16_t)(ns       & 0x1) << 8)
       | ((uint16_t)(cwr      & 0x1) << 7)
       | ((uint16_t)(ece      & 0x1) << 6)
       | ((uint16_t)(urg      & 0x1) << 5)
       | ((uint16_t)(ack      & 0x1) << 4)
       | ((uint16_t)(psh      & 0x1) << 3)
       | ((uint16_t)(rst      & 0x1) << 2)
       | ((uint16_t)(syn      & 0x1) << 1)
       | ((uint16_t)(fin      & 0x1) << 0);
}

#endif 

#ifndef MSG_COMMON_H_
#define MSG_COMMON_H_

#include <stdint.h>

typedef struct msg_hdr {
  uint32_t version;
  uint32_t seq;
  uint64_t sensor_usec;
  uint64_t sys_usec;
  uint64_t pub_usec;
  uint64_t reserved;
} __attribute__((packed)) msg_hdr_t;

#endif  // MSG_COMMON_H_

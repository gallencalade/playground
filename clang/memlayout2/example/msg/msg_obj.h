#ifndef MSG_OBJ_H_
#define MSG_OBJ_H_

#include "msg_common.h"

namespace msg {
namespace obj {

const int16_t kSize = 16;

struct Point {
  double x;
  double y;
} __attribute__((packed));

struct BoundingBox {
  float maxpt[3];
  float minpt[3];
} __attribute__((packed));

struct Object {
  uint8_t id;
  uint16_t num;
  Point objs1[20];
  Point objs2[kSize];
  BoundingBox bb1;
  BoundingBox bb2[30];
  BoundingBox bb3[kSize];
} __attribute__((packed));

}
}

#endif  // MSG_OBJ_H_

#ifndef MSG_INFER_H_
#define MSG_INFER_H_

#include "stdint.h"

namespace msg {
namespace infer {

enum Direction : int32_t {
  GOAHEAD = 0,
  RIGHT = 1,
  LEFT = 2,
};

enum Type : int32_t {
  RAMP = 0,
  TUNNEL = 1,
  ROADWORKS = 3,
};

struct Infer {
  int32_t in_geo;
  double dist;
  Direction direct;
  Type type;
  int32_t num;
} __attribute__((packed));

}
}

#endif  // MSG_INFER_H_

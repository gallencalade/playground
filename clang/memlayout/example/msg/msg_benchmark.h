#ifndef MSG_BENCHMARK_H_
#define MSG_BENCHMARK_H_

#include "msg_common.h"

namespace msg {
namespace benchmark {

struct ResultBench {
  uint8_t gaze_zone;
  uint32_t camera_id;
  float accuracy;
  float err_distance[17];
  float subpixel_mu[17];
  float subpixel_hist[17][20];
  float reserved[4];
} __attribute__((packed));

}
}

#endif  // MSG_BENCHMARK_SGBM_H_

#ifndef MSG_AREA_H_
#define MSG_AREA_H_

namespace msg {
namespace area {

struct Vector3 {
  double x;
  double y;
  double z;
} __attribute__((packed));

struct Quaternion {
  double x;
  double y;
  double z;
  double w;
} __attribute__((packed));

struct Pose {
  struct Vector3 posit;
  struct Quaternion orient;
};

}
}

#endif  // MSG_AREA_H_

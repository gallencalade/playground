#ifndef MSG_H_
#define MSG_H_

#include <cstdint>

extern "C" {

namespace nsa {

namespace nsb {

struct Sub {
  float f;
  double d;
  char c;
};

}

}

typedef int32_t INT32;

namespace ns1 {

namespace ns2 {

enum E {
  E1,
  E2,
  E3,
};

using SS = nsa::nsb::Sub;

struct Msg {
  char b;
  int a;
  float g;
  double k;
  float p[10];
  int c[20][40][80];
  double f[200][4];
  INT32 t32;
  SS s;
  union U {
    int u1;
    char u2[4];
  } u;
  union {
    int u1;
    char u2[4];
    struct {
      int aa;
    } cc;
  } v;
};

}

}

}

#endif // MSG_H_
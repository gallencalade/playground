#include "msg.h"
#include "publish.h"

#include "demo.h"

using demo::Demo;

using namespace ns1;

void Demo::run() {
  message::Message<ns2::Msg> message;
  publish(message);
}

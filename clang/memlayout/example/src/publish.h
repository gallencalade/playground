#ifndef PUBLISH_H_
#define PUBLISH_H_

namespace message {

template<typename T>
class Message {
  T t;
};

}

template<typename T>
void publish(const message::Message<T>&) {

}

#endif  // PUBLISH_H_

#ifndef EXPECTED_H
#define EXPECTED_H

template <typename T, typename E>
class expected {
 public:
  T& get() {
    if (!valid_) {
      throw std::logic_error("Invalid value");
    }

    return value_;
  }

  E& error() {
    if (!valid_) {
      throw std::logic_error("Invalid value");
    }

    return error_;
  }

 private:
  union {
    T value_;
    E error_;
  };

  bool valid_;
};

#endif  // EXPECTED_H

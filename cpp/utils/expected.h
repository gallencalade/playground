#ifndef EXPECTED_H
#define EXPECTED_H

#include <stdexcept>
#include <utility>

template <typename T, typename E>
class expected {
 public:
  template <typename... Args>
  static expected success(Args&& ...args) {
    expected result;
    result.valid_ = true;
    new (&result.value_)T(std::forward<Args>(args)...);
    return result;
  }

  template <typename... Args>
  static expected error(Args&& ...args) {
    expected result;
    result.valid_ = false;
    new (&result.error_)E(std::forward<Args>(args)...);
    return result;
  }

  expected(const expected& other)
    : valid_(other.valid_) {
      if (valid_) { new (&value_)T(other.value_); }
      else { new (&error_)T(other.error_); }
  }

  expected(expected&& other)
    : valid_(other.valid_) {
      if (valid_) { new (&value_)T(std::move(other.value_)); }
      else { new (&error_)T(std::move(other.error_)); }
  }

  expected& operator=(expected other) {
    swap(other);
    return *this;
  }

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
  void swap(expected& other) {
    using std::swap;
    if (valid_) {
      if (other.valid_) {
        swap(value_, other.value);
      } else {
        auto temp = std::move(other.error_);
        other.error_.~E();
        new (&other.value_)T(std::move(value_));
        value_.~T();
        new (&other.error_)E(std::move(temp));
        std::swap(valid_, other.valid_);
      }
    } else {
      if (other.valid_) { other.swap(*this); }
      else { swap(error_, other.error_); }
    }
  }

 private:
  union {
    T value_;
    E error_;
  };

  bool valid_;
};

#endif  // EXPECTED_H

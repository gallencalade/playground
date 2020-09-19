#include <stdexcept>

struct Database {
  Database() {
    static int instance_count { 0 };
    if (++instance_count > 1) {
      throw std::runtime_error("Cannot > 1");
    }
  }
};

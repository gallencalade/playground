#include <boost/memory_order.hpp>
#include <boost/atomic.hpp>
#include <boost/thread/mutex.hpp>

class Database {
 protected:
  Database() = default;

 public:
  static Database& instance() {
    Database* db = instance_.load(boost::memory_order_consume);
    if (!db) {
      boost::mutex::scoped_lock lock(mtx_);
      db = instance_.load(boost::memory_order_consume);
      if (!db) {
        db = new Database();
        instance_.store(db, boost::memory_order_release);
      }
    }

    return *db;
  }

 private:
  static boost::atomic<Database*> instance_;
  static boost::mutex mtx_;

  Database(const Database&);
  Database(Database&&);
  Database& operator=(const Database&);
  Database& operator=(Database&&);
};

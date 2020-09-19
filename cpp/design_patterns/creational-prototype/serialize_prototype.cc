#include <string>
#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/access.hpp>

struct Address {
 private:
  friend class boost::serialization::access;
  template<class Ar> void serialize(Ar& ar, const unsigned int version) {
    ar & street;
    ar & city;
    ar & suite;
  }

 public:
  std::string street;
  std::string city;
  int suite;
};

struct Contact {
 public:
  std::string name;
  Address* address = nullptr;

  ~Contact() {
    delete address;
    address = nullptr;
  }

 private:
  friend class boost::serialization::access;
  template<class Ar> void serialize(Ar& ar, const unsigned int version) {
    ar & name;
    ar & address;   // no *
  }
};

int main(int argc, const char* argv[]) {
  auto clone = [](const Contact& c) {
    std::ostringstream oss;
    boost::archive::text_oarchive oa(oss);
    oa << c;
    std::string s = oss.str();

    std::istringstream iss(oss.str());
    boost::archive::text_iarchive ia(iss);
    Contact result;
    ia >> result;
    return result;
  };

  Contact john{ "john", new Address{ "ST", "CT", 0 } };
  Contact jane = clone(john);
  jane.name = "jane";

  return 0;
}

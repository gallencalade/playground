#include <memory>
#include <string>

struct Address {
  Address(const std::string& street, const std::string& city, int suite)
    : street(street), city(city), suite(suite) {  }

  std::string street;
  std::string city;
  int suite;
};

struct Contact {
  std::string name;
  Address* address = nullptr;

  Contact(std::string&& name, Address* addr)
    : name(name), address(addr) {  }

  ~Contact() {
    delete address;
    address = nullptr;
  }

  Contact(const Contact& other)
    : name(other.name)
    , address(new Address(*other.address)) {  }

  Contact& operator=(const Contact& other) {
    if (this == &other) { return *this; }
    name = other.name;
    if (nullptr != address) { delete address; }
    address = other.address;
    return *this;
  }
};

struct EmployeeFactory {
  static std::unique_ptr<Contact> NewMainOffaceEmployee(
        std::string name, int suite) {
    return NewEmployee(name, suite, main);
  }

  static std::unique_ptr<Contact> NewAuxOfficeEmployee(
        std::string name, int suite) {
    return NewEmployee(name, suite, aux);
  }

 private:
  static std::unique_ptr<Contact> NewEmployee(
        std::string name, int suite, Contact& proto) {
    auto result = std::make_unique<Contact>(proto);
    result->name = name;
    result->address->suite = suite;
    return result;
  }

  static Contact main;
  static Contact aux;
};

Contact EmployeeFactory::main("", new Address{ "ST1", "CT", 0 });
Contact EmployeeFactory::aux("", new Address{ "ST2", "CT", 0 });

int main(int argc, char* argv[]) {
  auto john = EmployeeFactory::NewAuxOfficeEmployee("John", 123);
  auto jane = EmployeeFactory::NewMainOffaceEmployee("Jane", 125);

  return 0;
}

#include <iostream>
#include <string>

class PersonBuilder;

/**
 * \class Person
 * \brief The entity to build
 */
class Person {
 public:
  Person() = default;

  static PersonBuilder create();

  // address
  std::string street_address;
  std::string post_code;
  std::string city;

  // employment
  std::string company_name;
  std::string position;
  int annual_income = 0;
};

class PersonAddressBuilder;
class PersonJobBuilder;

/**
 * \class PersonBuilderBase
 * \brief Base class of all members builder.
 */
class PersonBuilderBase {
 protected:
  explicit PersonBuilderBase(Person& person) : person(person) {  }

 public:
  operator Person() { return std::move(person); }

  // builder facets
  PersonAddressBuilder lives() const;
  PersonJobBuilder works() const;

 protected:
  Person& person;     // This is a reference
};

/**
 * \class PersonBuilder
 * \brief Real Person builder, also inherits from PersonBuilderBase, but this
 *        class keeps Person.
 */
class PersonBuilder : public PersonBuilderBase {
 public:
  PersonBuilder() : PersonBuilderBase(p) {  }

  operator Person() { return p; }

 private:
  Person p;           // object being built
};

/* static */
PersonBuilder Person::create() { return PersonBuilder(); }

class PersonAddressBuilder : public PersonBuilderBase {
  typedef PersonAddressBuilder self;

 public:
  explicit PersonAddressBuilder(Person& person) : PersonBuilderBase(person) {  }

  self& at(const std::string& street_address) {
    person.street_address = street_address;
    return *this;
  }

  self& with_postcode(const std::string& post_code) {
    person.post_code = post_code;
    return *this;
  }

  self& in(const std::string& city) {
    person.city = city;
    return *this;
  }
};

class PersonJobBuilder : public PersonBuilderBase {
  typedef PersonJobBuilder self;

 public:
  explicit PersonJobBuilder(Person& person) : PersonBuilderBase(person) {  }

  self& at(const std::string& company_name) {
    person.company_name = company_name;
    return *this;
  }

  self& as_a(const std::string& position) {
    person.position = position;
    return *this;
  }

  self& earning(int annual_income) {
    person.annual_income = annual_income;
    return *this;
  }
};

PersonAddressBuilder PersonBuilderBase::lives() const {
  return PersonAddressBuilder(person);
}

PersonJobBuilder PersonBuilderBase::works() const {
  return PersonJobBuilder(person);
}

int main() {
  Person p = Person::create()
    .lives().at("Science and Technology")
            .with_postcode("53000")
            .in("ShenZhen")
    .works().at("BAT")
            .as_a("Programmer")
            .earning(1000);

  return 0;
}

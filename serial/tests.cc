#include "struct_pointer.h"

#include <iostream>
#include <cstring>

#include <gtest/gtest.h>

class alignas(8) Door {
 public:
  uint32_t DataSize() const {
    return sizeof(Door);
  }

  uint32_t Serialize(void* buf) const {
    Door* p = (Door*)buf;
    memcpy((void*)p, this, sizeof(Door));
    uint32_t data_size = sizeof(Door);

    return data_size;
  }

 public:
  int8_t color() const { return color_; }
  void set_color(int8_t c) { color_ = c; }

 private:
  int8_t color_;
};


TEST(Serialize, SingleClass) {
  Door door;
  door.set_color(127);

  uint32_t bufsize = door.DataSize();
  unsigned char* buf = new unsigned char[bufsize];
  EXPECT_EQ(door.Serialize(buf), bufsize);

  Door* p = (Door*)buf;
  EXPECT_EQ(p->color(), door.color());
  EXPECT_EQ(p->color(), 127);

  delete[] buf;
}

class alignas(8) House {
 public:
  uint32_t DataSize() const {
    return sizeof(House) + door1_->DataSize() + door2_->DataSize();
  }

  uint32_t Serialize(void* buf) const {
    House* p = (House*)buf;
    memcpy((void*)p, this, sizeof(House));
    uint32_t data_size = sizeof(House);

    data_size += door1_.Serialize((char*)buf + offsetof(House, door1_),
                                  data_size - offsetof(House, door1_));

    data_size += door2_.Serialize((char*)buf + offsetof(House, door2_),
                                  data_size - offsetof(House, door2_));

    return data_size;
  }

  int64_t location() const { return location_; }
  void set_location(int64_t l) { location_ = l; }

  const Door& door1() const { return door1_.Ref(); }
  Door& door1() { return door1_.Ref(); }

  const Door& door2() const { return door2_.Ref(); }
  Door& door2() { return door2_.Ref(); }

 private:
  StructPointer<Door> door1_;
  int64_t location_;
  StructPointer<Door> door2_;
};

TEST(Serialize, DoubleEmbedClassLv1) {
  House house;
  house.door1().set_color(64);
  house.set_location(10023);
  house.door2().set_color(120);

  uint32_t bufsize = house.DataSize();
  unsigned char* buf = new unsigned char[bufsize];
  EXPECT_EQ(house.Serialize(buf), bufsize);

  House* p = (House*)buf;

  EXPECT_EQ(p->door1().color(), house.door1().color());
  EXPECT_EQ(p->location(), house.location());
  EXPECT_EQ(p->door2().color(), house.door2().color());
  EXPECT_EQ(p->door1().color(), 64);
  EXPECT_EQ(p->location(), 10023);
  EXPECT_EQ(p->door2().color(), 120);


  delete[] buf;
}

class alignas(8) Car {
 public:
  uint32_t DataSize() const { return sizeof(Car); }

  uint32_t Serialize(void* buf) const {
    Car* p = (Car*)buf;
    memcpy((void*)p, this, sizeof(Car));
    uint32_t data_size = sizeof(Car);

    return data_size;
  }

 public:
  int16_t lic() const { return lic_; }
  void set_lic(int16_t l) { lic_ = l; }

 public:
  int16_t lic_;
};

class alignas(8) Person {
 public:
  uint32_t DataSize() const {
    return sizeof(Person) + house1_->DataSize() + car1_->DataSize() + house2_->DataSize();
  }

  uint32_t Serialize(void* buf) const {
    Person* p = (Person*)buf;
    memcpy((void*)p, this, sizeof(Person));
    uint32_t data_size = sizeof(Person);

    data_size += house1_.Serialize((char*)buf + offsetof(Person, house1_),
                                  data_size - offsetof(Person, house1_));
    data_size += car1_.Serialize((char*)buf + offsetof(Person, car1_),
                                  data_size - offsetof(Person, car1_));
    data_size += house2_.Serialize((char*)buf + offsetof(Person, house2_),
                                  data_size - offsetof(Person, house2_));

    return data_size;
  }

 public:
  int64_t name() const { return name_; }
  void set_name(int64_t n) { name_ = n; }

  House& house1() { return house1_.Ref(); }
  const House& house1() const { return house1_.Ref(); }

  bool gender() const { return gender_; }
  void set_gender(bool g) { gender_ = g; }

  Car& car1() { return car1_.Ref(); }
  const Car& car1() const { return car1_.Ref(); }

  House& house2() { return house2_.Ref(); }
  const House& house2() const { return house2_.Ref(); }

 private:
  int64_t name_;
  StructPointer<House> house1_;
  bool gender_;
  StructPointer<Car> car1_;
  StructPointer<House> house2_;
};

TEST(Serialize, DoubleEmbedClassLv2) {
  Person person;
  person.set_name(1111);
  person.house1().set_location(123);
  person.house1().door1().set_color(1);
  person.house1().door2().set_color(8);
  person.set_gender(false);
  person.car1().set_lic(1234);
  person.house2().set_location(555);
  person.house2().door1().set_color(99);
  person.house2().door2().set_color(36);

  uint32_t bufsize = person.DataSize();
  unsigned char* buf = new unsigned char[bufsize];
  person.Serialize(buf);
  Person* p = (Person*)buf;
  EXPECT_EQ(p->name(), person.name());
  EXPECT_EQ(p->house1().location(), person.house1().location());
  EXPECT_EQ(p->house1().door1().color(), person.house1().door1().color());
  EXPECT_EQ(p->house1().door2().color(), person.house1().door2().color());
  EXPECT_EQ(p->gender(), person.gender());
  EXPECT_EQ(p->car1().lic(), person.car1().lic());
  EXPECT_EQ(p->house2().location(), person.house2().location());
  EXPECT_EQ(p->house2().door1().color(), person.house2().door1().color());
  EXPECT_EQ(p->house2().door2().color(), person.house2().door2().color());
  EXPECT_EQ(p->name(), 1111);
  EXPECT_EQ(p->house1().location(), 123);
  EXPECT_EQ(p->house1().door1().color(), 1);
  EXPECT_EQ(p->house1().door2().color(), 8);
  EXPECT_EQ(p->gender(), false);
  EXPECT_EQ(p->car1().lic(), 1234);
  EXPECT_EQ(p->house2().location(), 555);
  EXPECT_EQ(p->house2().door1().color(), 99);
  EXPECT_EQ(p->house2().door2().color(), 36);

  delete[] buf;
}

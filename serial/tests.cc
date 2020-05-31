#include "struct_pointer.h"

#include <iostream>
#include <cstring>

#include <gtest/gtest.h>

//// Serialization Tests Cases
class alignas(8) Door {
 public:
  Door(int8_t color = {  }) : color_(color) {  }

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
  std::srand(777);
  door.set_color(std::rand() % INT8_MAX);

  uint32_t bufsize = door.DataSize();
  unsigned char* buf = new unsigned char[bufsize];
  EXPECT_EQ(bufsize, door.Serialize(buf));

  Door* p = (Door*)buf;
  EXPECT_EQ(p->color(), door.color());

  std::srand(777);
  EXPECT_EQ(p->color(), std::rand() % INT8_MAX);

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

TEST(Serialize, EmbedClassLv1) {
  House house;
  std::srand(8888);
  house.door1().set_color(std::rand() % INT8_MAX);
  house.set_location(std::rand() % INT64_MAX);
  house.door2().set_color(std::rand() % INT8_MAX);

  uint32_t bufsize = house.DataSize();
  unsigned char* buf = new unsigned char[bufsize];
  EXPECT_EQ(bufsize, house.Serialize(buf));

  House* p = (House*)buf;
  EXPECT_EQ(p->door1().color(), house.door1().color());
  EXPECT_EQ(p->location(), house.location());
  EXPECT_EQ(p->door2().color(), house.door2().color());

  std::srand(8888);
  EXPECT_EQ(p->door1().color(), std::rand() % INT8_MAX);
  EXPECT_EQ(p->location(), std::rand() % INT64_MAX);
  EXPECT_EQ(p->door2().color(), std::rand() % INT8_MAX);


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

TEST(Serialize, EmbedClassLv2) {
  Person person;
  std::srand(6666);
  person.set_name(std::rand() % INT64_MAX);
  person.house1().set_location(std::rand() % INT64_MAX);
  person.house1().door1().set_color(std::rand() % INT8_MAX);
  person.house1().door2().set_color(std::rand() % INT8_MAX);
  person.set_gender(std::rand() % 2);
  person.car1().set_lic(std::rand() % INT16_MAX);
  person.house2().set_location(std::rand() % INT64_MAX);
  person.house2().door1().set_color(std::rand() % INT8_MAX);
  person.house2().door2().set_color(std::rand() % INT8_MAX);

  uint32_t bufsize = person.DataSize();
  unsigned char* buf = new unsigned char[bufsize];
  EXPECT_EQ(bufsize, person.Serialize(buf));

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

  std::srand(6666);
  EXPECT_EQ(p->name(), std::rand() % INT64_MAX);
  EXPECT_EQ(p->house1().location(), std::rand() % INT64_MAX);
  EXPECT_EQ(p->house1().door1().color(), std::rand() % INT8_MAX);
  EXPECT_EQ(p->house1().door2().color(), std::rand() % INT8_MAX);
  EXPECT_EQ(p->gender(), std::rand() % 2);
  EXPECT_EQ(p->car1().lic(), std::rand() % INT16_MAX);
  EXPECT_EQ(p->house2().location(), std::rand() % INT64_MAX);
  EXPECT_EQ(p->house2().door1().color(), std::rand() % INT8_MAX);
  EXPECT_EQ(p->house2().door2().color(), std::rand() % INT8_MAX);

  delete[] buf;
}

TEST(Serialize, VectorFundamental) {
  Vector<int> vec(20);
  for (uint32_t i = 0; i < vec.Num(); ++i) {
    vec[i] = i * 11;
  }

  uint32_t bufsize = vec.DataSize();
  unsigned char* buf = new unsigned char[bufsize];
  EXPECT_EQ(bufsize, vec.Serialize(buf));

  Vector<int>* p = (Vector<int>*)buf;
  EXPECT_EQ(p->Num(), vec.Num());

  for (uint32_t i = 0; i < p->Num(); ++i) {
    EXPECT_EQ((*p)[i], vec[i]);
  }

  for (uint32_t i = 0; i < p->Num(); ++i) {
    EXPECT_EQ((*p)[i], i * 11);
  }

  delete[] buf;
}

TEST(Serialize, VectorSingleClass) {
  Vector<StructPointer<Door>> vec(20);
  for (uint32_t i = 0; i < vec.Num(); ++i) {
    vec[i].set_color(i);
  }

  uint32_t bufsize = vec.DataSize();
  unsigned char* buf = new unsigned char[bufsize];
  EXPECT_EQ(bufsize, vec.Serialize(buf));

  Vector<StructPointer<Door>>* p = (Vector<StructPointer<Door>>*)buf;
  for (uint32_t i = 0; i < p->Num(); ++i) {
    EXPECT_EQ((*p)[i].color(), vec[i].color());
  }
  for (uint32_t i = 0; i < p->Num(); ++i) {
    EXPECT_EQ((*p)[i].color(), i);
  }

  delete[] buf;
}

TEST(Serialize, VectorEmbedClassLv1) {
  Vector<StructPointer<House>> vec(20);
  std::srand(9999);
  for (uint32_t i = 0; i < vec.Num(); ++i) {
    vec[i].door1().set_color(std::rand() % INT8_MAX);
    vec[i].set_location(std::rand() % INT64_MAX);
    vec[i].door2().set_color(std::rand() % INT8_MAX);
  }

  uint32_t bufsize = vec.DataSize();
  unsigned char* buf = new unsigned char[bufsize];
  EXPECT_EQ(bufsize, vec.Serialize(buf));

  Vector<StructPointer<House>>* p = (Vector<StructPointer<House>>*)buf;
  for (uint32_t i = 0; i < p->Num(); ++i) {
    EXPECT_EQ((*p)[i].door1().color(), vec[i].door1().color());
    EXPECT_EQ((*p)[i].location(), vec[i].location());
    EXPECT_EQ((*p)[i].door2().color(), vec[i].door2().color());
  }

  std::srand(9999);
  for (uint32_t i = 0; i < p->Num(); ++i) {
    EXPECT_EQ((*p)[i].door1().color(), std::rand() % INT8_MAX);
    EXPECT_EQ((*p)[i].location(), std::rand() % INT64_MAX);
    EXPECT_EQ((*p)[i].door2().color(), std::rand() % INT8_MAX);
  }


  delete[] buf;
}

TEST(Serialize, VectorEmbedClassLv2) {
  Vector<StructPointer<Person>> vec(20);
  std::srand(6789);
  for (uint32_t i = 0; i < vec.Num(); ++i) {
    vec[i].set_name(std::rand() % INT64_MAX);
    vec[i].house1().set_location(std::rand() % INT64_MAX);
    vec[i].house1().door1().set_color(std::rand() % INT8_MAX);
    vec[i].house1().door2().set_color(std::rand() % INT8_MAX);
    vec[i].set_gender(std::rand() % 2);
    vec[i].car1().set_lic(std::rand() % INT16_MAX);
    vec[i].house2().set_location(std::rand() % INT64_MAX);
    vec[i].house2().door1().set_color(std::rand() % INT8_MAX);
    vec[i].house2().door2().set_color(std::rand() % INT8_MAX);
  }

  uint32_t bufsize = vec.DataSize();
  unsigned char* buf = new unsigned char[bufsize];
  EXPECT_EQ(bufsize, vec.Serialize(buf));

  Vector<StructPointer<Person>>* p = (Vector<StructPointer<Person>>*)buf;
  for (uint32_t i = 0; i < vec.Num(); ++i) {
    EXPECT_EQ((*p)[i].name(), vec[i].name());
    EXPECT_EQ((*p)[i].house1().location(), vec[i].house1().location());
    EXPECT_EQ((*p)[i].house1().door1().color(), vec[i].house1().door1().color());
    EXPECT_EQ((*p)[i].house1().door2().color(), vec[i].house1().door2().color());
    EXPECT_EQ((*p)[i].gender(), vec[i].gender());
    EXPECT_EQ((*p)[i].car1().lic(), vec[i].car1().lic());
    EXPECT_EQ((*p)[i].house2().location(), vec[i].house2().location());
    EXPECT_EQ((*p)[i].house2().door1().color(), vec[i].house2().door1().color());
    EXPECT_EQ((*p)[i].house2().door2().color(), vec[i].house2().door2().color());
  }

  std::srand(6789);
  for (uint32_t i = 0; i < vec.Num(); ++i) {
    EXPECT_EQ((*p)[i].name(), std::rand() % INT64_MAX);
    EXPECT_EQ((*p)[i].house1().location(), std::rand() % INT64_MAX);
    EXPECT_EQ((*p)[i].house1().door1().color(), std::rand() % INT8_MAX);
    EXPECT_EQ((*p)[i].house1().door2().color(), std::rand() % INT8_MAX);
    EXPECT_EQ((*p)[i].gender(), std::rand() % 2);
    EXPECT_EQ((*p)[i].car1().lic(), std::rand() % INT16_MAX);
    EXPECT_EQ((*p)[i].house2().location(), std::rand() % INT64_MAX);
    EXPECT_EQ((*p)[i].house2().door1().color(), std::rand() % INT8_MAX);
    EXPECT_EQ((*p)[i].house2().door2().color(), std::rand() % INT8_MAX);
  }

  delete[] buf;
}

TEST(Serialize, VectorVectorFundamental) {
  Vector<StructPointer<Vector<int>>> vec{
    { 10, 11, 12, 13, 14, },
    { 20, 21, 22, 23 } };
  EXPECT_EQ(5, vec[0].Num());
  EXPECT_EQ(4, vec[1].Num());

  for (uint32_t i = 0; i < vec[0].Num(); ++i) {
    EXPECT_EQ(10 + i, vec[0][i]);
  }
  for (uint32_t i = 0; i < vec[1].Num(); ++i) {
    EXPECT_EQ(20 + i, vec[1][i]);
  }

  uint32_t bufsize = vec.DataSize();
  unsigned char* buf = new unsigned char[bufsize];

  EXPECT_EQ(bufsize, vec.Serialize(buf));
  Vector<StructPointer<Vector<int>>>* p =
      (Vector<StructPointer<Vector<int>>>*)buf;
  EXPECT_EQ((*p)[0].Num(), vec[0].Num());
  EXPECT_EQ((*p)[1].Num(), vec[1].Num());
  for (uint32_t i = 0; i < vec[0].Num(); ++i) {
    EXPECT_EQ(10 + i, (*p)[0][i]);
  }
  for (uint32_t i = 0; i < vec[1].Num(); ++i) {
    EXPECT_EQ(20 + i, (*p)[1][i]);
  }

  delete[] buf;
}

TEST(Serialize, VectorVectorSingleClass) {
  Vector<StructPointer<Vector<StructPointer<Door>>>> vec{
    { Door(0), Door(1), Door(2), Door(3), Door(4) },
    { Door(10), Door(11), Door(12) }
  };

  EXPECT_EQ(5, vec[0].Num());
  EXPECT_EQ(3, vec[1].Num());
  for (uint32_t i = 0; i < vec[0].Num(); ++i) {
    EXPECT_EQ(i, vec[0][i].color());
  }
  for (uint32_t i = 0; i < vec[1].Num(); ++i) {
    EXPECT_EQ(10 + i, vec[1][i].color());
  }

  uint32_t bufsize = vec.DataSize();
  unsigned char* buf = new unsigned char[bufsize];

  EXPECT_EQ(bufsize, vec.Serialize(buf));
  Vector<StructPointer<Vector<StructPointer<Door>>>>* p =
      (Vector<StructPointer<Vector<StructPointer<Door>>>>*)buf;
  EXPECT_EQ((*p)[0].Num(), vec[0].Num());
  EXPECT_EQ((*p)[1].Num(), vec[1].Num());
  for (uint32_t i = 0; i < vec[0].Num(); ++i) {
    EXPECT_EQ(i, (*p)[0][i].color());
  }
  for (uint32_t i = 0; i < vec[1].Num(); ++i) {
    EXPECT_EQ(10 + i, (*p)[1][i].color());
  }

  delete[] buf;
}

TEST(Serialize, VectorVectorEmbedClassLv1) {

}

TEST(Serialize, VectorVectorEmbedClassLv2) {

}

//// StructPointer Tests Cases
TEST(StructPointer, StructPointerCopyAssign) {
  StructPointer<Door> door1;
  StructPointer<Door> door2(door1);
  StructPointer<Door> door3 = door2;

  StructPointer<House> house1;
  StructPointer<House> house2(house1);
  StructPointer<House> house3 = house2;

  StructPointer<Person> person1;
  StructPointer<Person> person2(person1);
  StructPointer<Person> person3 = person2;

  StructPointer<Vector<int>> vec1(10);
  StructPointer<Vector<int>> vec2(vec1);
  StructPointer<Vector<int>> vec3 = vec2;

  StructPointer<Vector<StructPointer<Door>>> vecdoor1(10);
  StructPointer<Vector<StructPointer<Door>>> vecdoor2(vecdoor1);
  StructPointer<Vector<StructPointer<Door>>> vecdoor3 = vecdoor2;

  StructPointer<Vector<StructPointer<House>>> vechouse1(10);
  StructPointer<Vector<StructPointer<House>>> vechouse2(vechouse1);
  StructPointer<Vector<StructPointer<House>>> vechouse3 = vechouse2;

  StructPointer<Vector<StructPointer<Person>>> vecperson1(10);
  StructPointer<Vector<StructPointer<Person>>> vecperson2(vecperson1);
  StructPointer<Vector<StructPointer<Person>>> vecperson3 = vecperson2;
}

//// Vector Tests Cases
TEST(Vector, VectorCopyAssign) {
  Vector<int> vec1(10);
  Vector<int> vec2(vec1);
  Vector<int> vec3 = vec2;

  Vector<StructPointer<Door>> vecdoor1(10);
  Vector<StructPointer<Door>> vecdoor2(vecdoor1);
  Vector<StructPointer<Door>> vecdoor3 = vecdoor2;

  Vector<StructPointer<House>> vechouse1(10);
  Vector<StructPointer<House>> vechouse2(vechouse1);
  Vector<StructPointer<House>> vechouse3 = vechouse2;

  Vector<StructPointer<Person>> vecperson1(10);
  Vector<StructPointer<Person>> vecperson2(vecperson1);
  Vector<StructPointer<Person>> vecperson3 = vecperson2;

  Vector<StructPointer<Vector<int>>> vecvec1(10);
  Vector<StructPointer<Vector<int>>> vecvec2(vecvec1);
  Vector<StructPointer<Vector<int>>> vecvec3 = vecvec2;

  Vector<StructPointer<Vector<StructPointer<Door>>>> vecvecdoor1(10);
  Vector<StructPointer<Vector<StructPointer<Door>>>> vecvecdoor2(vecvecdoor1);
  Vector<StructPointer<Vector<StructPointer<Door>>>> vecvecdoor3 = vecvecdoor2;

  Vector<StructPointer<Vector<StructPointer<House>>>> vecvechouse1(10);
  Vector<StructPointer<Vector<StructPointer<House>>>> vecvechouse2(vecvechouse1);
  Vector<StructPointer<Vector<StructPointer<House>>>> vecvechouse3 = vecvechouse2;

  Vector<StructPointer<Vector<StructPointer<Person>>>> vecvecperson1(10);
  Vector<StructPointer<Vector<StructPointer<Person>>>> vecvecperson2(vecvecperson1);
  Vector<StructPointer<Vector<StructPointer<Person>>>> vecvecperson3 = vecvecperson2;
}

TEST(Vector, VariableFundamental) {
  Vector<int> vec;
  for (uint32_t i = 0; i < 20; ++i) {
    vec.PushBack(i);
  }

  EXPECT_EQ(20, vec.Num());
  for (uint32_t i = 0; i < vec.Num(); ++i) {
    EXPECT_EQ(i, vec[i]);
  }
}

TEST(Vector, InitializerListFundamental) {
  Vector<int> vec{ 0, 10, 20, 30, 40, 50, 60, 70, 80, 90 };
  EXPECT_EQ(10, vec.Num());
  for (uint32_t i = 0; i < vec.Num(); ++i) {
    EXPECT_EQ(i * 10, vec[i]);
  }
}

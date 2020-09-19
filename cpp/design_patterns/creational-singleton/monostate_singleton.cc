class Printer {
 public:
  int get_id() const { return id_; }
  void set_id(int value) { id_ = value; }

 private:
  static int id_;
};

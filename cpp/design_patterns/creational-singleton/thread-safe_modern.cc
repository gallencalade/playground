class Database {
 protected:
  Database() = default;

 public:
  static Database& get() {
    static Database database;
    return database;
  }

  Database(const Database&) = delete;
  Database(Database&&) = delete;
  Database& operator=(const Database&) = delete;
  Database& operator=(Database&&) = delete;
};

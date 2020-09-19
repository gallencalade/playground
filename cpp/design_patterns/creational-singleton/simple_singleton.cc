struct Database {
  // Please dont create more than one instance
};

Database& get_database() {
  static Database database;
  return database;
}

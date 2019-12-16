#include <getopt.h>

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

using namespace std;

void help() {
  std::cout << std::endl;
  std::cout << "\tUsage: longopt [option] [param] ...\n"
               "\t\t--option A, -a\tOption A.\n"
               "\t\t--option B, -b\tOption B.\n"
               "\t\t--option C, -c\tOption C.\n"
               "\t\t--option D, -d\tOption D.\n"
            << std::endl;
}

std::string read_file(const std::string& file) {
  std::ifstream ifs(file, std::ios::binary);
  if (!ifs.is_open()) {
    throw std::runtime_error("Failed to open file: " + file);
  }

  return std::string((std::istreambuf_iterator<char>(ifs)),
                     std::istreambuf_iterator<char>());
}

bool check_arg(int atgc, char* argv[]) {
  return true;
}

int main(int argc, char* argv[]) {
  if (!check_arg(argc, argv)) {
    help();
    return -1;
  }

  const char* const shot_opts = "p:t:k:i:c:gv";
  struct option long_opts[] = {
    { "template", 1, nullptr, 't' },
    { "prikey", 1, nullptr, 'p' },
    { "in", 1, nullptr, 'i' },
    { "check", 1, nullptr, 'i' },
    { "generate", 0, nullptr, 'g' },
    { "verificate", 0, nullptr, 'v' },
    { nullptr, 0, nullptr, 0 },
  };

  std::string lic_tpl;
  std::string lic_in;
  std::string lic_pri_key;
  std::string lic_pub_key;
  bool lic_gen = false;
  bool lic_verify = false;
  char opt;
  while (-1 != (opt = getopt_long(argc, argv, shot_opts, long_opts, nullptr))) {
    switch (opt) {
      case 't':
        lic_tpl.assign(optarg);
        break;
      case 'i':
        lic_in.assign(optarg);
        break;
      case 'g':
        lic_gen = true;
        break;
      case 'v':
        lic_verify = true;
        break;
      case 'c':
        lic_pub_key.assign(optarg);
        break;
      case 'p':
        lic_pri_key.assign(optarg);
        break;
      default:
        return lictools_help();
    }
  }

  if (lic_gen == lic_verify) {
    return lictools_help();
  }

  if (lic_gen) {
    if (lic_tpl.empty() || lic_in.empty() || lic_pri_key.empty()) {
      return lictools_help();
    }
    std::string lic_in_content(read_file(lic_in));
    return lictools_generate(lic_tpl, lic_in_content, lic_pri_key, lic_pub_key,
          lic_in + ".lic");
  }

  if (lic_verify) {
    if (lic_in.empty()) {
      return lictools_help();
    }
    std::cout << "License to be Verificated: " << lic_in << std::endl;
    std::string lic_in_content(read_file(lic_in));
    std::cout << "=-=-=-=-=-=-=-=-= License Plain =-=-=-=-=-=-=-=-="
              << std::endl << lictools_verificate(lic_in_content, lic_pub_key)
              << std::endl;
  }

  return 0;
}

#include <getopt.h>

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

using namespace std;

void help() {
  std::cout << std::endl;
  std::cout << "\tGenerate toc in [TOC] field.\n"
               "\tThe Header1(#) is not generated.\n"
               "\tThe header # should be the first character of a line.\n"
               "\n\tUsage: gitbook_toc [option] [param] ...\n"
               "\t\t--input, -i\tInput file name.\n"
               "\t\t--output, -o\tOutput file name.\n"
               "\t\t--level, -c\tThe level to generate toc, 2 is default.\n"
            << std::endl;
}

bool check_arg(int argc, char* argv[]) {
  return true;
}

template<class ForwardIt, class T>
void replace(ForwardIt first, ForwardIt last,
             const T& old_value, const T& new_value) {
  for (; first != last; ++first) {
    if (*first == old_value) {
      *first = new_value;
    }
  }
}

static std::string str_toc("[TOC]");

int generate_toc(const std::string& file_input,
                 const std::string& file_output,
                 size_t level = 2) {

  std::ifstream ifs(file_input, std::ios::binary);
  if (!ifs.is_open()) {
    std::cerr << "Failed to open " << file_input << std::endl;
    return -1;
  }
  ifs.clear();
  std::ofstream ofs(file_output, std::ios::binary);
  if (!ofs.is_open()) {
    std::cerr << "Failed to open " << file_output << std::endl;
    return -1;
  }
  ofs.clear();

  std::string line;
  while (std::getline(ifs, line) && std::string::npos == line.find(str_toc)) {
    ofs << line << std::endl;
  }

  auto pos_toc = ifs.tellg();
  while (std::getline(ifs, line)) {
    auto pos_sharp = line.find_first_of('#');
    if (0 == pos_sharp) {
      auto pos_not_sharp = line.find_first_not_of('#');
      auto num_tab = pos_not_sharp - pos_sharp;
      if (num_tab <= level) {
        while (--num_tab) {
          ofs << ' ';
        }
        ofs << "* ";
        std::string header(line, pos_not_sharp);
        replace(header.begin() + header.find_first_not_of(' '), header.end(),
                ' ', '-');
        ofs << header << std::endl;
      }
    }
  }

  ifs.seekg(pos_toc);
  ofs << ifs.rdbuf();

  ifs.clear();
  ofs.clear();

  return 0;
}

int main(int argc, char* argv[]) {
  if (!check_arg(argc, argv)) {
    help();
    return -1;
  }

  const char* const shot_opts = "i:o:l:";
  struct option long_opts[] = {
    { "input", 1, nullptr, 'i' },
    { "output", 1, nullptr, 'o' },
    { "level", 0, nullptr, 'l' },
    { nullptr, 0, nullptr, 0 },
  };

  std::string file_input;
  std::string file_output;
  int level = 2;
  char opt;
  while (-1 != (opt = getopt_long(argc, argv, shot_opts, long_opts, nullptr))) {
    switch (opt) {
      case 'i':
        file_input.assign(optarg);
        break;
      case 'o':
        file_output.assign(optarg);
        break;
      case 'l':
        level = std::stoi(optarg);
        break;
      default:
        help();
        return -1;
    }
  }

  return generate_toc(file_input, file_output, level);
}

#include <iostream>
#include <vector>
#include <string>
#include <cctype>

class BetterFormattedText {
 public:
  BetterFormattedText(const std::string& s) : plain_text(s) {  }


  struct TextRange {
    int start, end;
    bool capitalize;

    bool covers(int position) const {
      return position >= start && position <= end;
    }
  };

  TextRange& get_range(int start, int end) {
    formatting.emplace_back(TextRange{ start, end });
    return * formatting.rbegin();
  }

  friend std::ostream& operator<<(std::ostream& os,
        const BetterFormattedText& obj) {
    std::string s;
    for (size_t i = 0; i < obj.plain_text.length(); ++i) {
      auto c = obj.plain_text[i];
      for (const auto& rng : obj.formatting) {
        if (rng.covers(i) && rng.capitalize) { c = std::toupper(c); }
        s += c;
      }
    }

    return os << s;
  }

 private:
  std::string plain_text;
  std::vector<TextRange> formatting;
};

int main() {
  BetterFormattedText bft("This is a brave new world");
  bft.get_range(10, 15).capitalize = true;
  std::cout << bft << std::endl;
}

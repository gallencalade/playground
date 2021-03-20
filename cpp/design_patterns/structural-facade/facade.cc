class ConsoleCreationParameters {
  int char_width{10};
  int char_height{14};
  int width{20};
  int height{30};
  bool fullscreen{false};
  bool create_default{true};
};

struct Console {
 Console(const ConsoleCreationParameters& ccp) {  }
};

// struct Console {
//   Console(int char_width,
//           int char_height,
//           int width,
//           int height,
//           bool fullscreen,
//           bool create_default) {  }
// };

#ifndef TEST_MAIN_CMDLINE_HPP
#define TEST_MAIN_CMDLINE_HPP
struct cmdline_parse {
  cmdline_parse(int argc, char** argv) {
    backtrace_flag = false;
    seed_given = false;
    seed_arg = 1023;
    unsigned i = 0;
    while (++i < argc) {
      if (argv[i][0] == '-') {
        switch (argv[i][1]) {
          case 's':
            seed_given = true;
            seed_arg = std::atoi(argv[++i]);
            break;
          case 'b':
            backtrace_flag = true;
            break;
          default:
            std::abort();
        }
      }
    }
  }
  bool backtrace_flag;
  bool seed_given;
  uint32_t seed_arg;
};
#endif

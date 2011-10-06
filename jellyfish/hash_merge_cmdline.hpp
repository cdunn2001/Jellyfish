#ifndef __MERGE_ARGS_HPP__
#define __MERGE_ARGS_HPP__

#include <yaggo.hpp>

class merge_args {
public:
  uint64_t                       buffer_size_arg;
  bool                           buffer_size_given;
  yaggo::string                  output_arg;
  bool                           output_given;
  uint32_t                       out_counter_len_arg;
  bool                           out_counter_len_given;
  uint64_t                       out_buffer_size_arg;
  bool                           out_buffer_size_given;
  bool                           verbose_flag;
  std::vector<const char *>      input_arg;
  typedef std::vector<const char *>::iterator input_arg_it;
  typedef std::vector<const char *>::const_iterator input_arg_const_it;

  enum {
    USAGE_OPT = 1000,
    OUT_COUNTER_LEN_OPT,
    OUT_BUFFER_SIZE_OPT
  };

  merge_args(int argc, char *argv[]) :
    buffer_size_arg(10000000), buffer_size_given(false),
    output_arg("mer_counts_merged.jf"), output_given(false),
    out_counter_len_arg(4), out_counter_len_given(false),
    out_buffer_size_arg(10000000), out_buffer_size_given(false),
    verbose_flag(false)
  {
    static struct option long_options[] = {
      {"buffer-size", 1, 0, 's'},
      {"output", 1, 0, 'o'},
      {"out-counter-len", 1, 0, OUT_COUNTER_LEN_OPT},
      {"out-buffer-size", 1, 0, OUT_BUFFER_SIZE_OPT},
      {"verbose", 0, 0, 'v'},
      {"help", 0, 0, 'h'},
      {"usage", 0, 0, USAGE_OPT},
      {"version", 0, 0, 'V'},
      {0, 0, 0, 0}
    };
    static const char *short_options = "hVs:o:v";

    std::string err;
#define CHECK_ERR(type,val,which) if(!err.empty()) { std::cerr << "Invalid " #type " '" << val << "' for [" which "]: " << err << "\n"; exit(1); }
    while(true) { 
      int index = -1;
      int c = getopt_long(argc, argv, short_options, long_options, &index);
      if(c == -1) break;
      switch(c) {
      case ':': 
        std::cerr << "Missing required argument for "
                  << (index == -1 ? std::string(1, (char)optopt) : std::string(long_options[index].name))
                  << std::endl;
        exit(1);
      case 'h':
        std::cout << usage() << "\n\n" << help() << std::endl;
        exit(0);
      case USAGE_OPT:
        std::cout << usage() << "\nUse --help for more information." << std::endl;
        exit(0);
      case 'V':
        print_version();
        exit(0);
      case '?':
        std::cerr << "Use --usage or --help for some help\n";
        exit(1);
      case 's':
        buffer_size_given = true;
        buffer_size_arg = yaggo::conv_uint<uint64_t>((const char *)optarg, err, false);
        CHECK_ERR(uint64_t, optarg, "-s, --buffer-size=Buffer length")
        break;
      case 'o':
        output_given = true;
        output_arg.assign(optarg);
        break;
      case OUT_COUNTER_LEN_OPT:
        out_counter_len_given = true;
        out_counter_len_arg = yaggo::conv_uint<uint32_t>((const char *)optarg, err, false);
        CHECK_ERR(uint32_t, optarg, "    --out-counter-len=uint32")
        break;
      case OUT_BUFFER_SIZE_OPT:
        out_buffer_size_given = true;
        out_buffer_size_arg = yaggo::conv_uint<uint64_t>((const char *)optarg, err, false);
        CHECK_ERR(uint64_t, optarg, "    --out-buffer-size=uint64")
        break;
      case 'v':
        verbose_flag = true;
        break;
      }
    }
    if(argc - optind < 2)
      error("Requires at least 2 arguments.");
    for( ; optind < argc; ++optind) {
      input_arg.push_back(argv[optind]);
    }
  }
#define merge_args_USAGE "Usage: jellyfish merge [options] input:c_string+"
  const char * usage() const { return merge_args_USAGE; }
  void error(const char *msg) { 
    std::cerr << "Error: " << msg << "\n" << usage()
              << "\nUse --help for more information"
              << std::endl;
    exit(1);
  }
#define merge_args_HELP "Merge jellyfish databases\n\n" \
  "Options (default value in (), *required):\n" \
  " -s, --buffer-size=Buffer length          Length in bytes of input buffer (10000000)\n" \
  " -o, --output=string                      Output file (mer_counts_merged.jf)\n" \
  "     --out-counter-len=uint32             Length (in bytes) of counting field in output (4)\n" \
  "     --out-buffer-size=uint64             Size of output buffer per thread (10000000)\n" \
  " -v, --verbose                            Be verbose (false)\n" \
  "     --usage                              Usage\n" \
  " -h, --help                               This message\n" \
  " -V, --version                            Version"

  const char * help() const { return merge_args_HELP; }
#define merge_args_HIDDEN "Hidden options:"

  const char * hidden() const { return merge_args_HIDDEN; }
  void print_version(std::ostream &os = std::cout) const {
#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "0.0.0"
#endif
    os << PACKAGE_VERSION << "\n";
  }
  void dump(std::ostream &os = std::cout) {
    os << "buffer_size_given:" << buffer_size_given << " buffer_size_arg:" << buffer_size_arg << "\n";
    os << "output_given:" << output_given << " output_arg:" << output_arg << "\n";
    os << "out_counter_len_given:" << out_counter_len_given << " out_counter_len_arg:" << out_counter_len_arg << "\n";
    os << "out_buffer_size_given:" << out_buffer_size_given << " out_buffer_size_arg:" << out_buffer_size_arg << "\n";
    os << "verbose_flag:" << verbose_flag << "\n";
    os << "input_arg:" << yaggo::vec_str(input_arg) << "\n";
  }
private:
};

#endif // __MERGE_ARGS_HPP__"

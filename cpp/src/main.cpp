#include <boost/program_options.hpp>
#include <iostream>
#include <project_defines.h>

using namespace boost::program_options;

/**
 * Params struct for storing runtime params
 */
struct Params {
  bool run = false;
  std::string filename;
} params; //!< global Params object

/**
 * Function for arguments parsing
 * @param argc arguments count from stdin
 * @param argv arguments array
 */
inline void parseParams(int argc, char *argv[]) {
  try {
    options_description desc{"Options"};
    desc.add_options()("help,h", "Help screen")(
        "input,i", value<std::vector<std::string>>(),
        "Input file")("output,o", value<std::string>(), "Output file")(
        "margin,m", value<unsigned>(),
        "Margin of maps overlap in quants "
        "[min no of symbols for overlap to be meaningfull]");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    /* Parse options */
    if (vm.count("help")) {
      std::cout << desc << '\n';
    } else {
      if (vm.count("input") > 0) {
        params.run      = true;
        params.filename = vm["input"].as<std::string>();
      }
    }
  } catch (const boost::program_options::error &ex) {
    std::cerr << ex.what() << " !\n";
  }
}

/**
 * Main function
 * @return 0 on success
 */
int main(int argc, char *argv[]) {
  std::cout << "Prototype OM algorithm - OMA\n"
            << "by Stawczyk Przemyslaw" << std::endl; // flush stream

  parseParams(argc, argv);
  if (params.run) {
    std::cout << "RUNNING\n";
    // TODO : container for OM data
    // TODO : transform and save ?
  } else {
    // NOTE : temp playgorund
  }
  return 0;
}

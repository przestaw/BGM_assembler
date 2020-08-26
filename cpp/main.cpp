#include <algorithms/assembly_task.h>
#include <boost/program_options.hpp>
#include <error/exeception.h>
#include <iostream>

using namespace boost::program_options;

/**
 * Params struct for storing runtime params
 */
struct Params {
  bool run = false;
  std::string input_file;
  std::string output_file;
  unsigned margin = 0;
};

/**
 * Function for arguments parsing
 * @param argc arguments count from stdin
 * @param argv arguments array
 */
inline Params parseParams(int argc, char *argv[]) {
  Params params;
  try {
    options_description desc{"Options"};
    desc.add_options()("help,h", "Help screen")("input,i", value<std::string>(),
                                                "Input file")(
        "output,o", value<std::string>(),
        "Output file")("margin,m", value<unsigned>(),
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
        if (vm.count("output") == 0) {
          throw oma::error::missing_argument("output");
        } else {
          params.output_file = vm["output"].as<std::string>();
        }

        params.run        = true;
        params.input_file = vm["input"].as<std::string>();

        if (vm.count("margin") == 0) {
          throw oma::error::missing_argument("margin");
        } else {
          params.margin = vm["margin"].as<unsigned>();
        }
      } else {
        throw oma::error::missing_argument("input");
      }
    }
  } catch (const boost::program_options::error &ex) {
    std::cerr << ex.what() << " !\n";
  }
  return params;
}

/**
 * Main function
 * @return 0 on success
 */
int main(int argc, char *argv[]) {
  std::cout << "Prototype OM algorithm - OMA\n"
            << "by Stawczyk Przemyslaw" << std::endl; // flush stream

  Params params = parseParams(argc, argv);
  if (params.run) {
    std::cout << "RUNNING\n";

    oma::AssemblyTask::Params assemblyParams(
        params.input_file, params.output_file, 1, params.margin, 0.5, 0.1);
    oma::AssemblyTask assemblyTask(assemblyParams);

    assemblyTask.run(true);
  }

  return 0;
}

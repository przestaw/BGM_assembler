#include <algorithms/assembly_task.h>
#include <boost/program_options.hpp>
#include <error/exeception.h>
#include <iostream>

using namespace boost::program_options;

/**
 * ProgramParams struct for storing runtime params
 */
struct ProgramParams {
  bool _run                         = false;
  bool _verbose                     = false;
  oma::AssemblyTask::Params _params = oma::AssemblyTask::Params("", "");
};

/**
 * Function for arguments parsing
 * @param argc arguments count from stdin
 * @param argv arguments array
 */
inline ProgramParams parseParams(int argc, char *argv[]) {
  ProgramParams params;
  try {
    options_description desc{"Options"};
    desc.add_options()("help,h", "Help screen");
    desc.add_options()("input,i", value<std::string>(), "Input file");
    desc.add_options()("output,o", value<std::string>(), "Output file");
    desc.add_options()("margin_quant", value<unsigned>(),
                       "Margin of maps overlap in quants "
                       "[min no of symbols for overlap to be meaningful]");
    desc.add_options()("margin_part", value<double>(),
                       "Margin of maps part overlap"
                       "[min part of map overlap to be meaningful]");
    desc.add_options()("thresh", value<double>(),
                       "Max part of differences between two maps "
                       "to be considered meaningful");
    desc.add_options()("threads,t", value<unsigned>()->default_value(1),
                       "No of threads used for parallel algorithm parts"
                       "[0 will force to use max detected threads, higher "
                       "values will create desired threads count]");
    desc.add_options()("verbose,v", "Verbose output [current state]");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    /* Parse options */
    if (vm.count("help")) {
      std::cout << desc << '\n';
    } else {
      auto checkPresence = [&vm](const std::string &name) {
        if (!vm.count(name)) {
          throw oma::error::missing_argument(name);
        }
      };

      checkPresence("input");
      checkPresence("output");
      checkPresence("margin_quant");
      checkPresence("margin_part");
      checkPresence("thresh");
      // threads has default value

      params._run    = true;
      params._params = oma::AssemblyTask::Params(
          vm["input"].as<std::string>(), vm["output"].as<std::string>(),
          vm["threads"].as<unsigned>(), vm["margin_quant"].as<unsigned>(),
          vm["margin_part"].as<double>(), vm["thresh"].as<double>());
      params._verbose = vm.count("verbose");
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

  ProgramParams params = parseParams(argc, argv);
  if (params._run) {
    std::cout << "RUNNING\n";

    //    oma::AssemblyTask::Params assemblyParams(
    //        params.input_file, params.output_file, 1, params.margin, 0.5,
    //        0.1);
    oma::AssemblyTask assemblyTask(params._params);

    assemblyTask.run(params._verbose);
  }

  return 0;
}

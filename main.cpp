#include <algorithms/assembly_task.h>
#include <boost/program_options.hpp>
#include <error/exeception.h>
#include <iostream>

using namespace boost::program_options;

/**
 * ProgramParams struct for storing runtime params
 */
struct ProgramParams {
  explicit ProgramParams(variables_map vm);

  bool verbose_                     = false;
  oma::AssemblyTask::Params params_ = oma::AssemblyTask::Params("", "");
};

ProgramParams::ProgramParams(variables_map vm) {
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
  // folds has default value

  // folds cannot be less than 1
  unsigned folds = 1;
  if (vm["folds"].as<unsigned>() > folds) {
    folds = vm["folds"].as<unsigned>();
  }

  this->params_  = oma::AssemblyTask::Params(vm["input"].as<std::string>(),
                                            vm["output"].as<std::string>(),
                                            vm["threads"].as<unsigned>(),
                                            vm["margin_quant"].as<unsigned>(),
                                            vm["margin_part"].as<double>(),
                                            vm["thresh"].as<double>(),
                                            folds);
  this->verbose_ = vm.count("verbose");
}

/**
 * Function for arguments parsing
 * @param argc arguments count from stdin
 * @param argv arguments array
 */
inline std::optional<ProgramParams> parseParams(int argc, char *argv[]) {
  try {
    options_description desc{"Options"};
    desc.add_options()("help,h", "Help screen");
    desc.add_options()("input,i", value<std::string>(), "Input file");
    desc.add_options()("output,o", value<std::string>(), "Output file");
    desc.add_options()("margin_quant",
                       value<unsigned>(),
                       "Margin of maps overlap in quants "
                       "[min no of symbols for overlap to be meaningful]");
    desc.add_options()("margin_part",
                       value<double>(),
                       "Margin of maps part overlap"
                       "[min part of map overlap to be meaningful]");
    desc.add_options()("thresh",
                       value<double>(),
                       "Max part of differences between two maps "
                       "to be considered meaningful");
    desc.add_options()("threads,t",
                       value<unsigned>()->default_value(1),
                       "No of threads used for parallel algorithm parts"
                       "[0 will force to use max detected threads, higher "
                       "values will create desired threads count]");
    desc.add_options()("folds,f",
                       value<unsigned>()->default_value(1),
                       "No of folds [repeated assemblies]");
    desc.add_options()("verbose,v", "Verbose output [current state]");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    /* Parse options */
    if (vm.count("help")) {
      std::cout << desc << '\n';
      return std::optional<ProgramParams>();
    } else {
      // NOTE : constructor can throw, as RAII
      //        isocpp.org/wiki/faq/big-picture
      return std::optional<ProgramParams>(ProgramParams(vm));
    }
  } catch (const boost::program_options::error &ex) {
    std::cerr << ex.what() << " !\n";
    return std::optional<ProgramParams>();
  }
}

/**
 * Main function
 * @return 0 on success
 */
int main(int argc, char *argv[]) {
  std::cout << "Prototype OM algorithm - BGM_asm\n"
            << "by Stawczyk Przemyslaw" << std::endl; // flush stream

  auto params = parseParams(argc, argv);
  if (params.has_value()) {
    std::cout << "RUNNING\n";

    oma::AssemblyTask assemblyTask(params.value().params_);

    assemblyTask.run(params.value().verbose_);
  }

  return 0;
}

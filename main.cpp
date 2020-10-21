#include <algorithms/assembly_task.h>
#include <boost/program_options.hpp>
#include <common/observer.h>
#include <error/exeception.h>
#include <iostream>
#include <utility>

using namespace boost::program_options;
using namespace bgm;

/**
 * ProgramParams struct for storing runtime params
 */
class ProgramParams : public AssemblyParams {
public:
  ProgramParams(bool verbose, AssemblyParams params)
      : AssemblyParams(std::move(params)), verbose_(verbose) {}

  [[nodiscard]] bool isVerbose() const { return verbose_; }

private:
  bool verbose_;
};

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
    desc.add_options()("marker_preference",
                       value<bool>()->default_value(true),
                       "preference of marker (true) or empty place (false) "
                       "during consensus when 50% of maps has 1 and 50% has 0");
    desc.add_options()("reuse_maps",
                       "Reuse rmaps with contigs [from at least 2 rmaps] in each fold");
    desc.add_options()("verbose,v", "Verbose output");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    /* Parse options */
    if (vm.count("help")) {
      std::cout << desc << '\n';
      return std::optional<ProgramParams>();
    } else {
      /* NOTE : constructor can throw, as RAII
       *        isocpp.org/wiki/faq/big-picture
       */
      auto checkPresence = [&vm](const std::string &name) {
        if (!vm.count(name)) {
          throw bgm::error::missing_argument(name);
        }
      };

      checkPresence("input");
      checkPresence("output");
      checkPresence("margin_quant");
      checkPresence("margin_part");
      checkPresence("thresh");
      // threads has default value
      // folds has default value
      // marker_preference has default value

      // folds cannot be less than 1
      unsigned folds = 1;
      if (vm["folds"].as<unsigned>() > folds) {
        folds = vm["folds"].as<unsigned>();
      }

      auto params  = AssemblyTask::Params(vm["input"].as<std::string>(),
                                         vm["output"].as<std::string>(),
                                         vm["threads"].as<unsigned>(),
                                         vm["margin_quant"].as<unsigned>(),
                                         vm["margin_part"].as<double>(),
                                         vm["thresh"].as<double>(),
                                         folds,
                                         vm["marker_preference"].as<bool>(),
                                         static_cast<bool>(vm.count("reuse_maps")));
      bool verbose = vm.count("verbose");

      return ProgramParams(verbose, params);
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
    // slice ProgramParams to assembly params
    // NOLINTNEXTLINE(cppcoreguidelines-slicing) - slicing is intended
    AssemblyTask assemblyTask(static_cast<AssemblyParams>(params.value()));

    if (params->isVerbose()) {
      // attach progress bar
      assemblyTask.attachObserver(boost::make_shared<bgm::Observer>());

      std::cout << "RUNNING" << std::endl; // std::endl to flush stream
    }

    // run command
    assemblyTask.execute();

    if (params->isVerbose()) {
      std::cout << "\nDONE" << std::endl;
    }
  }

  return 0;
}

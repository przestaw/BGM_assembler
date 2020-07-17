#include <boost/program_options.hpp>
#include <iostream>
#include <project_defines.h>

#include <align_algorithm.h>
#include <data/file_loader.h>
#include <data/maps_container.h>
#include <error/exeception.h>
#include <include/c++/7/fstream>

using namespace boost::program_options;

/**
 * Params struct for storing runtime params
 */
struct Params {
  bool run = false;
  std::string filename;
  unsigned margin = 0;
}; //!< global Params object

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
      if (vm.count("margin") == 0) {
        throw oma::error::missing_argument("margin");
      } else {
        params.margin = vm["margin"].as<unsigned>();
      }
      if (vm.count("input") > 0) {
        params.run      = true;
        params.filename = vm["input"].as<std::string>();
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
  if (params.run == true) {
    std::cout << "RUNNING\n";

    std::ifstream file;
    file.open(params.filename);
    auto maps = oma::FileLoader::loadFile(file);
    file.close();

    oma::MapsContainer mapsContainer(maps);

    oma::AlignAlgorithm aligner(params.margin);

    aligner.align_maps_single(mapsContainer);

    auto lambdaL = [](const oma::BinaryMap &map) {
      if (map.getLeftNeighbour().first) {
        return "L : " + map.getLeftNeighbour().first->getIdentifier() + " "
               + std::to_string(map.getLeftNeighbour().second);
      } else {
        return std::string("L : XX");
      }
    };
    auto lambdaR = [](const oma::BinaryMap &map) {
      if (map.getRightNeighbour().first) {
        return "R : " + map.getRightNeighbour().first->getIdentifier() + " "
               + std::to_string(map.getRightNeighbour().second);
      } else {
        return std::string("R : XX");
      }
    };

    for (const auto &it : mapsContainer) {
      std::cout << it.getIdentifier() << "\n\t" << lambdaL(it) << "\n\t"
                << lambdaR(it) << "\n";
    }

  } else {
    // NOTE : temp playgorund
    return -1;
  }
  return 0;
}

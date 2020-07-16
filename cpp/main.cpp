#include <boost/program_options.hpp>
#include <iostream>
#include <project_defines.h>

#include <align_algorithm.h>
#include <data/file_loader.h>
#include <data/maps_container.h>
#include <include/c++/7/fstream>

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
    std::string filename =
        "./data/testmaps/2Mb_ecoli_2000quant_500len_errorfree.binmaps";
    std::ifstream file;
    file.open(filename);
    auto maps = oma::FileLoader::loadFile(file);
    file.close();

    for (auto &it : maps) {
      std::cout << it.getIdentifier() << "\n\t" << it.getMap() << '\n';
    }

    oma::MapsContainer mapsContainer(maps);

    oma::AlignAlgorithm aligner(350);

    aligner.align_maps_single(mapsContainer);

    auto lambdaL = [](oma::BinaryMap map) {
      if (map.getLeftNeighbour().first) {
        return "L : " + map.getLeftNeighbour().first->getIdentifier() + " "
               + std::to_string(map.getLeftNeighbour().second);
      } else {
        return std::string("L : XX");
      }
    };
    auto lambdaR = [](oma::BinaryMap map) {
      if (map.getRightNeighbour().first) {
        return "R : " + map.getRightNeighbour().first->getIdentifier() + " "
               + std::to_string(map.getRightNeighbour().second);
      } else {
        return std::string("R : XX");
      }
    };

    for (auto &it : mapsContainer) {
      std::cout << it.getIdentifier() << "\n\t" << lambdaL(it) << "\n\t"
                << lambdaR(it) << "\n";
    }
  }
  return 0;
}

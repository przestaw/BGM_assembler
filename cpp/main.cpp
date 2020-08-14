#include <algorithms/assembly_task.h>
#include <boost/program_options.hpp>
#include <data/file_loader.h>
#include <error/exeception.h>
#include <iostream>
#include <project_defines.h>

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

// void temp_output(const oma::MapsContainer &mapsContainer,
//                 const std::string &output_filename) {
//  auto lambdaL = [&](const oma::BinaryMap &map) {
//    if (map.getLeftNeighbour().first >= 0) {
//      return "L : "
//             + mapsContainer[map.getLeftNeighbour().first].getIdentifier() + "
//             "
//             + std::to_string(map.getLeftNeighbour().second);
//    } else {
//      return std::string("L : XX");
//    }
//  };
//  auto lambdaR = [&](const oma::BinaryMap &map) {
//    if (map.getRightNeighbour().first >= 0) {
//      return "R : "
//             + mapsContainer[map.getRightNeighbour().first].getIdentifier()
//             + " " + std::to_string(map.getRightNeighbour().second);
//    } else {
//      return std::string("R : XX");
//    }
//  };
//
//  std::ofstream output;
//  output.open(output_filename);
//
//  for (const auto &it : mapsContainer) {
//    output << it.getIdentifier() << "\n\t" << lambdaL(it) << "\n\t"
//           << lambdaR(it) << "\n";
//  }
//
//  output.close();
//}
//
// std::string print_map_list(const oma::MapsContainer &mapsContainer,
//                           const oma::BinaryMap &map) {
//  if (map.getRightNeighbour().first >= 0) {
//    return mapsContainer[map.getRightNeighbour().first].getIdentifier() + "\n"
//           + print_map_list(mapsContainer,
//                            mapsContainer[map.getRightNeighbour().first]);
//  } else {
//    return std::string("# --\t-- #\n");
//  }
//}
//
// void temp_output_2(const oma::MapsContainer &mapsContainer,
//                   const std::string &output_filename) {
//  std::ofstream output;
//  output.open(output_filename);
//
//  for (const auto &map : mapsContainer) {
//    if (map.getLeftNeighbour().first < 0) {
//      output << mapsContainer[map.getRightNeighbour().first].getIdentifier()
//                    + "\n" + print_map_list(mapsContainer, map);
//    }
//  }
//
//  output.close();
//}

std::pair<int64_t, size_t> test(oma::BinaryMap::Bitset ref,
                                oma::BinaryMap::Bitset tested) {
  // TODO : check or assure that maps are longer than minOverlap
  //        i.e. filter too short maps
  const size_t max_shift = 1 + std::min(ref.size(), tested.size()) - 100;
  // init best as worst numerically possible
  size_t best        = std::numeric_limits<size_t>::max();
  int64_t best_shift = 0;

  std::cout << "max shift = " << max_shift << '\n';

  if (ref.size() <= tested.size()) {
    std::size_t diff = tested.size() - ref.size();
    for (size_t shift = 1; shift < max_shift; ++shift) {
      // NOTE : bitset has different direction -> lowest & highest position
      // NOTE : use diff to move R ends together
      oma::BinaryMap::Bitset shiftedTest = tested >> (shift + diff);
      // resize to correct value
      shiftedTest.resize(tested.size() - (shift + diff));
      // resize to equal lengths
      if (shiftedTest.size() < ref.size()) {
        ref.resize(shiftedTest.size());
      }
      std::cout << "xor\n";
      std::cout << "ref " << ref.size() << "\n";
      printBit(ref);
      std::cout << "shiftedTest " << shiftedTest.size() << "\n";
      printBit(shiftedTest);

      shiftedTest ^= ref;
      size_t xor_result = shiftedTest.count();

      std::cout << "xor_result " << xor_result << "\n";
      printBit(shiftedTest);

      if (xor_result < shiftedTest.size() / 5) {
        // TODO : SCORE ???
        if (best > xor_result) {
          best       = xor_result;
          best_shift = shift;
        }
      }
    }
  } else { // reference is longer
    std::size_t diff = ref.size() - tested.size();
    // NOTE : bitset has different direction -> lowest & highest position
    // NOTE : use diff to move R ends together
    ref = ref >> diff;

    for (size_t shift = 1; shift < max_shift; ++shift) {
      // NOTE : bitset has different direction -> lowest & highest position
      oma::BinaryMap::Bitset shiftedTest = tested >> shift;
      // resize to correct value
      shiftedTest.resize(tested.size() - shift);
      // resize to equal lengths
      if (shiftedTest.size() < ref.size()) {
        ref.resize(shiftedTest.size());
      }
      std::cout << "xor\n";
      std::cout << "ref " << ref.size() << "\n";
      printBit(ref);
      std::cout << "shiftedTest " << shiftedTest.size() << "\n";
      printBit(shiftedTest);

      shiftedTest ^= ref;
      size_t xor_result = shiftedTest.count();

      std::cout << "xor_result " << xor_result << "\n";
      printBit(shiftedTest);

      if (xor_result < shiftedTest.size() / 5) {
        // TODO : SCORE ???
        if (best > xor_result) {
          best       = xor_result;
          best_shift = shift;
        }
      }
    }
  }
  return std::make_pair(best_shift, best);
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
        params.input_file, params.output_file, 1, params.margin, 0.1);
    oma::AssemblyTask assemblyTask(assemblyParams);

    assemblyTask.run(true);
  } else {
    std::string filer = "> 0_500 Escherichia coli str. K-12 substr. MG1655\n"
                        "010101110101000010001011001000110001000101100100000000"
                        "00000000100001000100000010\n"
                        "011010000001000100000001100000010001010011000001100011"
                        "10001001010001000010000100\n"
                        "001001000000100010000000110011100000110010000000100000"
                        "10001100000001100010001101\n"
                        "010000101101011000100000011000000000000000010000010000"
                        "01100000000000000000000100\n"
                        "100000000110100000011001000000000000000100000000000001"
                        "11100010100000010011100000\n"
                        "001001011100110111000011001100001000000111000011000000"
                        "00000101100001110000000000\n"
                        "00000000110000000101\n"
                        "> 100_600 Escherichia coli str. K-12 substr. MG1655\n"
                        "000110000001000101001100000110001110001001010001000010"
                        "00010000100100000010001000\n"
                        "000011001110000011001000000010000010001100000001100010"
                        "00110101000010110101100010\n"
                        "000001100000000000000001000001000001100000000000000000"
                        "00010010000000011010000001\n"
                        "100100000000000000010000000000000111100010100000010011"
                        "10000000100101110011011100\n"
                        "001100110000100000011100001100000000000101100001110000"
                        "00000000000000110000000101\n"
                        "001100010011010000101100000000000001001001000001100000"
                        "01000011000000001111100010\n"
                        "00101010000011000000";

    std::stringstream stream(filer);

    auto resu = oma::FileLoader::loadFile(stream);
    printBit(resu[0].getMap());
    printBit(resu[1].getMap());
  }
  //    auto lambdaL = [&](const oma::BinaryMap &map) {
  //      if (map.getLeftNeighbour().first >= 0) {
  //        return "L : "
  //               + mapsContainer[map.getLeftNeighbour().first].getIdentifier()
  //               + " " + std::to_string(map.getLeftNeighbour().second);
  //      } else {
  //        return std::string("L : XX");
  //      }
  //    };
  //    auto lambdaR = [&](const oma::BinaryMap &map) {
  //      if (map.getRightNeighbour().first >= 0) {
  //        return "R : "
  //               +
  //               mapsContainer[map.getRightNeighbour().first].getIdentifier()
  //               + " " + std::to_string(map.getRightNeighbour().second);
  //      } else {
  //        return std::string("R : XX");
  //      }
  //    };
  //
  //    for (const auto &it : mapsContainer) {
  //      std::cout << it.getIdentifier() << "\n\t" << lambdaL(it) << "\n\t"
  //                << lambdaR(it) << "\n";
  //    }
  //  }

  return 0;
}

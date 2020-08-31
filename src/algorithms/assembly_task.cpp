//
// Created by przemek on 12.08.2020.
//

#include <algorithms/assembly_task.h>
#include <data/file_loader.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <utility>

using namespace oma;

AssemblyTask::Params::Params(std::string inputFile, std::string outputFile,
                             unsigned int threads, unsigned int minOverlap,
                             double threshold)
    : _inputFile(std::move(inputFile)), _outputFile(std::move(outputFile)),
      _threads(threads), _minOverlap(minOverlap), _threshold(threshold) {}

AssemblyTask::AssemblyTask(AssemblyTask::Params params)
    : _params(std::move(params)), _aligner(_maps, _params.getMinOverlap()),
      _consensus(_maps) {}

void AssemblyTask::run(bool verbose) {
  auto print = [](const std::string &what) {
    std::time_t t = std::time(nullptr);
    std::cout << what + " at time : "
              << std::put_time(std::localtime(&t), "%F %T %C") << "\n";
  };
  if (verbose) {
    print("loading maps");
  }
  loadMaps();
  if (verbose) {
    print("aligning maps");
  }
  alignMaps();
  if (verbose) {
    print("performing consensus");
  }
  performConsensus();
  if (verbose) {
    print("writing result");
  }
  writeResult();
}

void AssemblyTask::loadMaps() {
  std::ifstream input;
  input.open(_params.getInputFile());

  auto loaded = MapsContainer(FileLoader::loadFile(input, _params.getMinMapLen()));
  // change container to loaded maps
  std::swap(_maps, loaded);

  input.close();
}

void AssemblyTask::alignMaps() { _aligner.alignMapsSingle(); }

void AssemblyTask::performConsensus() { _consensus.runConsensusSingle(); }

void AssemblyTask::writeResult() {
  /*
   * NOTE : TEMPORARY FOR TESTING
   * FIXME : REMOVE ADDITIONAL OUTPUT
   */

  // NOTE : detailed align output
  std::function<void(const std::string &)> outDetails =
      [&](const std::string &outFile) {
        auto lambdaL = [&](const oma::BinaryMap &map) {
          if (map.getLeftNeighbour().first >= 0) {
            return "L : " + _maps[map.getLeftNeighbour().first].getIdentifier()
                   + " " + std::to_string(map.getLeftNeighbour().second);
          } else {
            return std::string("L : XX");
          }
        };
        auto lambdaR = [&](const oma::BinaryMap &map) {
          if (map.getRightNeighbour().first >= 0) {
            return "R : " + _maps[map.getRightNeighbour().first].getIdentifier()
                   + " " + std::to_string(map.getRightNeighbour().second);
          } else {
            return std::string("R : XX");
          }
        };

        std::ofstream output;
        output.open(outFile);

        for (const auto &it : _maps) {
          output << it.getIdentifier() << "\n\t" << lambdaL(it) << "\n\t"
                 << lambdaR(it) << "\n";
        }

        output.close();
      };

  // NOTE : draft contig align output
  std::function<void(const std::string &)> outDrafts =
      [&](const std::string &outFile) {
        std::ofstream output;
        output.open(outFile);

        std::function<std::string(const oma::BinaryMap &)> mapListStr =
            [&](const oma::BinaryMap &map) {
              if (map.getRightNeighbour().first >= 0) {
                return _maps[map.getRightNeighbour().first].getIdentifier()
                       + "\n"
                       + mapListStr(_maps[map.getRightNeighbour().first]);
              } else {
                return std::string("# --\t-- #\n");
              }
            };

        for (const auto &map : _maps) {
          if (map.getLeftNeighbour().first < 0) {
            output << _maps[map.getRightNeighbour().first].getIdentifier()
                          + "\n" + mapListStr(map);
          }
        }

        output.close();
      };

  // NOTE : result output
  std::function<void(const std::string &)> outContigs =
      [&](const std::string &outFile) {
        std::ofstream output;
        output.open(outFile);

        for (auto &map : _consensus.getConsensusMaps()) {
          output << map.toFastaRecord();
        }

        output.close();
      };

  // TODO : possible choice for output files
  outDetails(_params.getOutputFile() + ".details");
  // outDrafts(_params.getOutputFile() + ".drafts");
  outContigs(_params.getOutputFile());
}

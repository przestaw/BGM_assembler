//
// Created by przemek on 12.08.2020.
//

#include <algorithms/assembly_task.h>
#include <data/file_loader.h>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <utility>

using namespace oma;

AssemblyTask::Params::Params(std::string inputFile, std::string outputFile,
                             unsigned int threads, unsigned int minOverlap,
                             double minPart, double threshold, unsigned repeats)
    : _inputFile(std::move(inputFile)), _outputFile(std::move(outputFile)),
      _threads(threads), _minOverlap(minOverlap), _minPart(minPart),
      _threshold(threshold), _repeats(repeats) {}

AssemblyTask::AssemblyTask(AssemblyTask::Params params)
    : _params(std::move(params)), _aligner(_maps, _params.getMinOverlap()),
      _consensus(_maps) {}

void AssemblyTask::run(bool verbose) {
  if (verbose) {
    runVerbose();
  } else {
    runQuiet();
  }
}

double AssemblyTask::runPerfTest() {
  loadMaps();

  auto begin = std::chrono::system_clock::now();

  for (unsigned i = 1; i <= _params.getRepeats(); ++i) {
    if (_maps.size() > 1) {
      alignMaps();
      performConsensus();

      _maps.resetAndReplace(_consensus.getConsensusMaps());
      _consensus.reset();
    } else {
      break;
    }
  }

  auto end = std::chrono::system_clock::now();

  writeResult();

  return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
             .count()
         / 1000.0;
}

void AssemblyTask::runQuiet() {
  loadMaps();

  for (unsigned i = 1; i <= _params.getRepeats(); ++i) {
    if (_maps.size() > 1) {
      alignMaps();
      performConsensus();

      _maps.resetAndReplace(_consensus.getConsensusMaps());
      _consensus.reset();
    } else {
      break;
    }
  }

  writeResult();
}

void AssemblyTask::runVerbose() {
  auto print = [](const std::string &what) {
    std::time_t t = std::time(nullptr);
    std::cout << what + " at time : "
              << std::put_time(std::localtime(&t), "%F %T %C") << "\n";
  };
  print("loading maps");
  loadMaps();

  for (unsigned i = 1; i <= _params.getRepeats(); ++i) {
    if (_maps.size() > 1) {
      print("aligning maps " + std::to_string(i) + " ");
      alignMaps();
      print("performing consensus " + std::to_string(i) + " ");
      performConsensus();

      _maps.resetAndReplace(_consensus.getConsensusMaps());
      _consensus.reset();
    } else {
      print("One map left ");
      break;
    }
  }

  print("writing result");
  writeResult();
}

void AssemblyTask::loadMaps() {
  std::ifstream input;
  input.open(_params.getInputFile());

  auto loaded =
      MapsContainer(FileLoader::loadFile(input, _params.getMinMapLen()));
  // change container to loaded maps
  std::swap(_maps, loaded);

  input.close();
}

void AssemblyTask::alignMaps() {
  _aligner.alignMapsMulti(_params.getThreads());
}

void AssemblyTask::performConsensus() {
  _consensus.runMultiConsensus(_params.getThreads());
}

void AssemblyTask::writeResult() {
  // NOTE : result output
  std::function<void(const std::string &)> outContigs =
      [&](const std::string &outFile) {
        std::ofstream output;
        output.open(outFile);

        for (auto &map : _maps) { output << map.toFastaRecord(); }

        output.close();
      };

  // NOTE : sort contigs before output
  std::sort(_maps.begin(), _maps.end(),
            [](const BinaryMap &a, const BinaryMap &b) -> bool {
              return a.getMap().size() > b.getMap().size();
            });

  outContigs(_params.getOutputFile());
}

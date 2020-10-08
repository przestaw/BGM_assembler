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

AssemblyTask::Params::Params(std::string inputFile,
                             std::string outputFile,
                             unsigned int threads,
                             unsigned int minOverlap,
                             double minPart,
                             double threshold,
                             unsigned repeats)
    : inputFile_(std::move(inputFile)), outputFile_(std::move(outputFile)),
      threads_(threads), minOverlap_(minOverlap), minPart_(minPart),
      threshold_(threshold), repeats_(repeats) {}

AssemblyTask::AssemblyTask(AssemblyTask::Params params)
    : params_(std::move(params)), aligner_(maps_, params_.getMinOverlap()),
      consensus_(maps_) {}

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

  for (unsigned i = 1; i <= params_.getRepeats(); ++i) {
    if (maps_.size() > 1) {
      alignMaps();
      performConsensus();

      maps_.resetAndReplace(consensus_.getConsensusMaps());
      consensus_.reset();
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

  for (unsigned i = 1; i <= params_.getRepeats(); ++i) {
    if (maps_.size() > 1) {
      alignMaps();
      performConsensus();

      maps_.resetAndReplace(consensus_.getConsensusMaps());
      consensus_.reset();
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

  for (unsigned i = 1; i <= params_.getRepeats(); ++i) {
    if (maps_.size() > 1) {
      print("aligning maps " + std::to_string(i) + " ");
      alignMaps();
      print("performing consensus " + std::to_string(i) + " ");
      performConsensus();

      maps_.resetAndReplace(consensus_.getConsensusMaps());
      consensus_.reset();
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
  input.open(params_.getInputFile());

  auto loaded =
      MapsContainer(FileLoader::loadFile(input, params_.getMinMapLen()));
  // change container to loaded maps
  std::swap(maps_, loaded);

  input.close();
}

void AssemblyTask::alignMaps() {
  aligner_.alignMapsMulti(params_.getThreads());
}

void AssemblyTask::performConsensus() {
  consensus_.runMultiConsensus(params_.getThreads());
}

void AssemblyTask::writeResult() {
  // NOTE : result output
  std::function<void(const std::string &)> outContigs =
      [&](const std::string &outFile) {
        std::ofstream output;
        output.open(outFile);

        for (auto &map : maps_) { output << map.toFastaRecord(); }

        output.close();
      };

  // NOTE : sort contigs before output
  std::sort(maps_.begin(),
            maps_.end(),
            [](const BinaryMap &a, const BinaryMap &b) -> bool {
              return a.getMap().size() > b.getMap().size();
            });

  outContigs(params_.getOutputFile());
}

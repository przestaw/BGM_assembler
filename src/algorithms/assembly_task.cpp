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

AssemblyTask::AssemblyTask(AssemblyTask::Params params)
    : params_(std::move(params)), aligner_(maps_, params_.getMinOverlap()),
      consensus_(maps_, params_.isMarkerPreference()) {
  // preapre fold - so maps contain what is desired for next iteration
  prepareNextFold_ = [&](bool reuseMaps) {
    if (reuseMaps) {
      auto newMaps = consensus_.getConsensusMapsAndReset();
      maps_.insert(maps_.end(),
                   std::make_move_iterator(newMaps.begin()),
                   std::make_move_iterator(newMaps.end()));
    } else {
      maps_.resetAndReplace(consensus_.getConsensusMaps());
      consensus_.reset();
    }
  };
  // perform fold
  fold_ = [&](){
    aligner_.alignMapsMulti(params_.getThreads());
    consensus_.runMultiConsensus(params_.getThreads());
  };

}

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
      aligner_.alignMapsMulti(params_.getThreads());
      consensus_.runMultiConsensus(params_.getThreads());

      maps_.resetAndReplace(consensus_.getConsensusMaps());
      consensus_.reset();
    } else {
      break;
    }
  }

  auto end = std::chrono::system_clock::now();

  writeResultToFile();

  return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
             .count()
         / 1000.0;
}

void AssemblyTask::runQuiet() {
  loadMaps();

  for (unsigned i = 1; i <= params_.getRepeats(); ++i) {
    if (maps_.size() > 1) {
      aligner_.alignMapsMulti(params_.getThreads());
      consensus_.runMultiConsensus(params_.getThreads());

      maps_.resetAndReplace(consensus_.getConsensusMaps());
      consensus_.reset();
    } else {
      break;
    }
  }

  writeResultToFile();
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
      aligner_.alignMapsMulti(params_.getThreads());
      print("performing consensus " + std::to_string(i) + " ");
      consensus_.runMultiConsensus(params_.getThreads());

      // maps_.resetAndReplace(consensus_.getConsensusMaps());
      // consensus_.reset();

      // take result
      auto newMaps = consensus_.getConsensusMapsAndReset();
      maps_.insert(maps_.end(),
                   std::make_move_iterator(newMaps.begin()),
                   std::make_move_iterator(newMaps.end()));
    } else {
      print("One map left ");
      break;
    }
  }

  print("writing result");
  writeResultToFile();
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

void AssemblyTask::writeResultToFile() {
  // NOTE : sort contigs before output
  std::sort(maps_.begin(),
            maps_.end(),
            [](const BinaryMap &a, const BinaryMap &b) -> bool {
              return a.getMap().size() > b.getMap().size();
            });

  // NOTE : result output
  std::ofstream output;
  output.open(params_.getOutputFile());

  for (auto &map : maps_) {
    if (map.isContig()) {
      output << map.toFastaRecord();
    }
  }

  output.close();
}

void AssemblyTask::operator()(mt4cpp::Progress &p) {
  const auto repeats = params_.getRepeats();
  const auto step    = 0.8 / repeats;
  loadMaps();

  p.setProgress(0.1);

  for (unsigned i = 1; i <= repeats; ++i) {
    if (maps_.size() > 1) {
      fold_();
      p.setProgress(0.1 + step * i);
      prepareNextFold_(params_.isReuseMaps());
    } else {
      break;
    }

  }

  p.setProgress(0.9);
  writeResultToFile();
  p.setProgress(1.);
}

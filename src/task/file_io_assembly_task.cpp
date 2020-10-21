//
// Created by przemek on 12.08.2020.
//

#include <data/file_loader.h>
#include <data/result_writer.hpp>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <task/file_io_assembly_task.h>
#include <utility>

using namespace bgm;

FileIOAssemblyTask::FileIOAssemblyTask(const std::string &input,
                                       const std::string &output,
                                       FileIOAssemblyTask::Params params)
    : AbstractTask(params, MapsContainer()), inputFile_(input), outputFile_(output) {}

void FileIOAssemblyTask::loadMaps() {
  std::ifstream input;
  input.open(inputFile_);

  auto loaded = MapsContainer(FileLoader::loadFile(input, params_.getMinMapLen()));
  // change container to loaded maps
  std::swap(maps_, loaded);

  input.close();
}

void FileIOAssemblyTask::writeResultToFile() {
  // NOTE : sort contigs before output
  std::sort(
      maps_.begin(), maps_.end(), [](const BinaryMap &a, const BinaryMap &b) -> bool {
        return a.getMap().size() > b.getMap().size();
      });

  // NOTE : result output
  ResultWriter::writeMapsToFile(
      outputFile_, maps_.begin(), maps_.end(), ResultWriter::contigsPredicate);
}

void FileIOAssemblyTask::operator()(mt4cpp::Progress &p) {
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

double FileIOAssemblyTask::profileAlgorithm() {
  // load maps
  loadMaps();

  // save start time
  auto begin = std::chrono::system_clock::now();

  // run
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

  // save end time
  auto end = std::chrono::system_clock::now();

  // write result
  writeResultToFile();

  // calc time adn return
  return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
         / 1000.0;
}

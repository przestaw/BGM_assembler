//
// Created by przestaw on 21.10.2020.
//

#include <task/assembly_task.h>
#include <data/file_loader.h>
#include <data/result_writer.hpp>
#include <functional>
#include <utility>

bgm::AssemblyTask::AssemblyTask(std::vector<BinaryMap> maps, Params params)
    : AbstractTask(params, MapsContainer(std::move(maps))) {}

void bgm::AssemblyTask::operator()(mt4cpp::Progress &p) {
  // calc step
  const auto repeats = params_.getRepeats();
  const auto step    = 1. / repeats;

  // run
  for (unsigned i = 1; i <= repeats; ++i) {
    if (maps_.size() > 1) {
      fold_();
      // increase progress after each fold
      p.setProgress(0.1 + step * i);
      prepareNextFold_(params_.isReuseMaps());
    } else {
      break;
    }
  }

  // finished -> progress = 1.
  p.setProgress(1.);
}

double bgm::AssemblyTask::profileAlgorithm() {
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

  // calc time adn return
  return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
         / 1000.0;
}

vector<bgm::BinaryMap> bgm::AssemblyTask::getResult() {
  vector<bgm::BinaryMap> ret;

  // filter
  for (const auto &map : maps_) {
    if (bgm::ResultWriter::contigsPredicate(map)) {
      ret.emplace_back(map);
    }
  }

  // sort
  std::sort(ret.begin(), ret.end(), [](const BinaryMap &a, const BinaryMap &b) -> bool {
    return a.getMap().size() > b.getMap().size();
  });

  // return
  return ret;
}

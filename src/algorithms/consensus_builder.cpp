//
// Created by przemek on 8/7/20.
//

#include <algorithms/consensus_builder.h>
#include <functional>
#include <iostream>
#include <thread>

using namespace bgm;

ConsensusBuilder::ConsensusBuilder(MapsContainer &maps,
                                   bool markerPreference,
                                   bool processAll)
    : conMaps_(maps), markerPreferenceFlag_(markerPreference),
      deductMarker_((markerPreference ? ConsensusBuilder::markerPreference_
                                      : ConsensusBuilder::noMarkerPreference_)),
      processAll_(processAll) {}

const std::vector<BinaryMap> &ConsensusBuilder::getConsensusMaps() const {
  return consensusMaps_;
}

void ConsensusBuilder::breakCycles() {
  for (auto &map : conMaps_) {
    if (map.getRightNeighbour().getMap() < 0) {
      // mark all from acyclic contig
      map.setMark();
      int tmp = map.getLeftNeighbour().getMap();
      // linear contig - because of neighbours structure
      while (tmp >= 0) {
        conMaps_[tmp].setMark();
        tmp = conMaps_[tmp].getLeftNeighbour().getMap();
      }
    }
  }

  for (size_t i = 0; i < conMaps_.size(); ++i) {
    // if not marked
    if (!conMaps_[i].isMark()) {
      breakOneCycle(i);
    }
  }
}

void ConsensusBuilder::breakOneCycle(int map) {
  int tmp     = map;
  double qual = bgm::BinaryMap::Neighbour::notAligned().getQuality();
  int right   = -1;
  // find largest shift
  do {
    conMaps_[tmp].setMark();

    if (qual > conMaps_[map].getLeftNeighbour().getQuality()) {
      qual  = conMaps_[map].getLeftNeighbour().getQuality();
      right = conMaps_[map].getLeftNeighbour().getMap();
    }

    tmp = conMaps_[tmp].getLeftNeighbour().getMap();
  } while (tmp != map);

  // break cycle
  if (right >= 0) {
    conMaps_.breakLink(conMaps_[right].getLeftNeighbour().getMap(), right);
  }
}

void ConsensusBuilder::runConsensusSingle() {
  breakCycles(); // break cycles before finding contigs
  detectContigs();

  for (const auto &i : contigs_) { makeConsensus(i); }
}

void ConsensusBuilder::runMultiConsensus(unsigned int threadsCount) {
  const auto processorCount = std::thread::hardware_concurrency();

  if (threadsCount == 0) {
    threadsCount = processorCount;
  }

  if (threadsCount == 1) {
    // single thread
    runConsensusSingle();
  } else {
    breakCycles(); // break cycles before finding contigs
    detectContigs();

    // multi thread
    std::vector<std::thread> threads;

    // allocate memory for threads
    threads.reserve(threadsCount);
    // create threads
    for (unsigned i = 0; i < threadsCount; ++i) {
      threads.emplace_back(&ConsensusBuilder::workerThread, this);
    }

    // join threads
    for (auto &workerThread : threads) { workerThread.join(); }
  }
}

void ConsensusBuilder::workerThread() {
  bool flag = true;

  auto getContig = [&]() {
    std::unique_lock guard(congigLock_);
    if (!contigs_.empty()) {
      const auto ret = contigs_.back();
      contigs_.pop_back();
      return ret;
    } else {
      flag = false;
      return size_t(0);
    }
  };

  size_t index = getContig();

  while (flag) {
    makeConsensus(index);
    index = getContig();
  }
}

void ConsensusBuilder::detectContigs() {
  // NOTE : if on top to branch once !
  if (processAll_) {
    for (size_t map = 0; map < conMaps_.size(); ++map) {
      if (conMaps_[map].getRightNeighbour().getMap() < 0) {
        contigs_.emplace_back(map);
      }
    }
  } else {
    // filter to contain only sequences of 2 or more maps
    for (size_t map = 0; map < conMaps_.size(); ++map) {
      if (conMaps_[map].getRightNeighbour().getMap() < 0) {
        if (countLenL(map) > 1) {
          contigs_.emplace_back(map);
        }
      }
    }
  }
}

bool ConsensusBuilder::votePosition(std::list<std::pair<std::size_t, int>> &voters) {
  std::size_t count = voters.size();
  std::size_t ones  = 0;

  // incremented inside loop
  for (auto iter = voters.begin(); iter != voters.end();) {
    // decrement and test position
    --(iter->first);
    if (conMaps_[iter->second].getMap().test(iter->first)) {
      ++ones;
    }

    // erase if reached beginning -> reverse iteration
    if (iter->first == 0) {
      iter = voters.erase(iter);
    } else {
      ++iter;
    }
  }

  // 1's priority, true = 1, false = 0
  return deductMarker_(count, ones);
}

void ConsensusBuilder::makeConsensus(int lastMap) {
  BinaryMap::Bitset contig;

  // voters for consensus
  std::list<std::pair<std::size_t, int>> voters;
  // add first voter
  voters.emplace_back(conMaps_[lastMap].getMap().size(), lastMap);
  // set counter to next voter
  std::size_t dist = conMaps_[lastMap].getLeftNeighbour().getShift();

  // run till no voter left
  while (!voters.empty()) {

    if (dist == 0) {
      // add next map 'voter'
      while (dist == 0 && conMaps_[lastMap].getLeftNeighbour().getMap() >= 0) {
        lastMap = conMaps_[lastMap].getLeftNeighbour().getMap();
        dist    = conMaps_[lastMap].getLeftNeighbour().getShift();
        voters.emplace_back(conMaps_[lastMap].getMap().size(), lastMap);
      }
    }
    // decrement distance to next voter
    --dist;

    // vote
    bool consensusResult = votePosition(voters);
    contig.push_back(consensusResult);
  }

  addReadyMap(contig);
}

void ConsensusBuilder::addReadyMap(const BinaryMap::Bitset &readyMap) {
  std::unique_lock lock(consensusLock);
  BinaryMap contig =
      BinaryMap("Contig " + std::to_string(consensusMaps_.size()), readyMap);
  contig.setContig(); // mark as contig
  consensusMaps_.push_back(std::move(contig));
}

void ConsensusBuilder::reset() {
  this->consensusMaps_.clear();
  this->contigs_.clear();
}

std::vector<BinaryMap> ConsensusBuilder::getConsensusMapsAndReset() {
  vector<BinaryMap> retConsensusMaps; // empty consensusMaps
  std::swap(retConsensusMaps, this->consensusMaps_);
  this->contigs_.clear();
  return retConsensusMaps;
}

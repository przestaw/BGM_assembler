//
// Created by przemek on 8/7/20.
//
#include <algorithms/consensus_builder.h>
#include <iostream>

using namespace oma;

ConsensusBuilder::ConsensusBuilder(MapsContainer &maps) : conMaps(maps) {}

const std::vector<BinaryMap> &ConsensusBuilder::getConsensusMaps() const {
  return consensusMaps;
}

void ConsensusBuilder::breakCycles() {
  for (auto &map : conMaps) {
    if (map.getRightNeighbour().first < 0) {
      // mark all from acyclic contig
      map.mark = true;
      int tmp  = map.getLeftNeighbour().first;
      // linear contig - because of neighbours structure
      while (tmp >= 0) {
        conMaps[tmp].mark = true;
        tmp               = conMaps[tmp].getLeftNeighbour().first;
      }
    }
  }

  for (auto &map : conMaps) {
    // if not marked
    if (!map.mark) {}
  }
}

void ConsensusBuilder::breakOneCycle(int map) {
  int tmp          = map;
  unsigned lenOvlp = 0;
  int right        = -1;
  // find largest shift
  do {
    conMaps[tmp].mark = true;

    if (lenOvlp < conMaps[map].getLeftNeighbour().second) {
      lenOvlp = conMaps[map].getLeftNeighbour().second;
      right   = conMaps[map].getLeftNeighbour().first;
    }

    tmp = conMaps[tmp].getLeftNeighbour().first;
  } while (tmp != map);
  // break cycle

  // TODO : use index instead of pointers
  //        -> rewrite pointers use
}

void ConsensusBuilder::runSingleConsensus() {
  detectContigs(); // TODO : implement and use

  // NOTE: temporary way of iterate over "contigs"
  for (size_t map = 0; map < conMaps.size(); ++map) {
    if (conMaps[map].getRightNeighbour().first < 0) {
      makeConsensus(map);
    }
  }
}

void ConsensusBuilder::detectContigs() {}

bool ConsensusBuilder::votePosition(
    std::list<std::pair<std::size_t, int>> &voters) {
  std::size_t count = voters.size();
  std::size_t ones  = 0;

  // incremented inside loop
  for (auto iter = voters.begin(); iter != voters.end();) {
    // decrement and test position
    if (conMaps[iter->second].getMap().test(--iter->first)) {
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
  return (count - ones) <= ones;
}

void ConsensusBuilder::makeConsensus(int lastMap) {
  BinaryMap::Bitset contig;

  // voters for consensus
  std::list<std::pair<std::size_t, int>> voters;
  // add first voter
  voters.emplace_back(conMaps[lastMap].getMap().size(), lastMap);
  // set counter to next voter
  std::size_t dist = 0;
  dist             = conMaps[lastMap].getLeftNeighbour().second;

  // NOTE : shift is realative to what ?
  //        -> find all L_neighbours as it's mutual relation

  // run till no voter left
  while (!voters.empty()) {

    if (dist > 0) {
      // decrement distance to next voter
      --dist;
    } else {
      // add next map 'voter'
      while (dist == 0 && conMaps[lastMap].getLeftNeighbour().first >= 0) {
        dist    = conMaps[lastMap].getLeftNeighbour().second;
        lastMap = conMaps[lastMap].getLeftNeighbour().first;
        voters.emplace_back(conMaps[lastMap].getMap().size(), lastMap);
      }
    }

    // vote
    bool consensusResult = votePosition(voters);
    contig.push_back(consensusResult);
  }

  addReadyMap(contig);
}

void ConsensusBuilder::addReadyMap(const BinaryMap::Bitset &readyMap) {
  std::unique_lock lock(consensusLock);
  consensusMaps.emplace_back("Contig " + std::to_string(readyMap.size()),
                             readyMap);
}

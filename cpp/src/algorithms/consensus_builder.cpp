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

  for (size_t i = 0; i < conMaps.size(); ++i) {
    // if not marked
    if (!conMaps[i].mark) {
      breakOneCycle(i);
    }
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
  conMaps.breakLink(conMaps[right].getLeftNeighbour().first, right);
}

void ConsensusBuilder::runConsensusSingle() {
  breakCycles(); // break cycles before finding contigs
  // detectContigs(); // TODO : implement and use

  // NOTE: temporary way of iterate over "contigs"
  for (size_t map = 0; map < conMaps.size(); ++map) {
    if (conMaps[map].getRightNeighbour().first < 0) {
      makeConsensus(map);
    }
  }
}

void ConsensusBuilder::detectContigs() {
  for (size_t map = 0; map < conMaps.size(); ++map) {
    if (conMaps[map].getRightNeighbour().first < 0) {
      // "create" contig -> for consensus
      // NOTE : could be list of most right maps or special structure for faster
      //        consensus
    }
  }
}

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
  std::size_t dist = conMaps[lastMap].getLeftNeighbour().second;

  // NOTE : shift is realative to RIGHT overhang
  //        -> find all L_neighbours as it's mutual relation

  // run till no voter left
  while (!voters.empty()) {

    if (dist == 0) {
      // add next map 'voter'
      while (dist == 0 && conMaps[lastMap].getLeftNeighbour().first >= 0) {
        lastMap = conMaps[lastMap].getLeftNeighbour().first;
        dist    = conMaps[lastMap].getLeftNeighbour().second;
        voters.emplace_back(conMaps[lastMap].getMap().size(), lastMap);
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
  consensusMaps.emplace_back("Contig " + std::to_string(consensusMaps.size()),
                             readyMap);
}

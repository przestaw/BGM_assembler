//
// Created by przemek on 8/7/20.
//
#include <algorithms/consensus_builder.h>
#include <iostream>

using namespace oma;

ConsensusBuilder::ConsensusBuilder(MapsContainer &maps) : connectedMaps(maps) {}

const std::vector<BinaryMap> &ConsensusBuilder::getConsensusMaps() const {
  return consensusMaps;
}

void ConsensusBuilder::breakCycles() {
  for (auto &map : connectedMaps) {
    if (map.getRightNeighbour().first == nullptr) {
      // mark all from acyclic contig
      map.mark = true;
      auto tmp = map.getLeftNeighbour().first;
      // linear contig - because of neighbours structure
      while (tmp) {
        tmp->mark = true;
        tmp       = tmp->getLeftNeighbour().first;
      }
    }
  }

  for (auto &map : connectedMaps) {
    // if not marked
    if (!map.mark) {}
  }
}

void ConsensusBuilder::breakOneCycle(BinaryMap *map) {
  auto tmp         = map;
  unsigned lenOvlp = 0;
  BinaryMap *right = nullptr;
  // find largest shift
  do {
    tmp->mark = true;

    if (lenOvlp < map->getLeftNeighbour().second) {
      lenOvlp = map->getLeftNeighbour().second;
      right   = map->getLeftNeighbour().first;
    }

    tmp = tmp->getLeftNeighbour().first;
  } while (tmp != map);
  // break cycle

  // TODO : use index instead of pointers
  //        -> rewrite pointers use
}

void ConsensusBuilder::runSingleConsensus() {
  detectContigs(); // TODO : implement and use

  // NOTE: temporary way of iterate over "contigs"
  for (auto &map : connectedMaps) {
    if (map.getRightNeighbour().first == nullptr) {
      makeConsensus(&map);
      std::cout << '\n' << map.getIdentifier() << '\n';
    }
  }
}

void ConsensusBuilder::detectContigs() {}

bool votePosition(std::list<std::pair<std::size_t, BinaryMap *>> &voters) {
  std::size_t count = voters.size();
  std::size_t ones  = 0;
  std::cout << "-\n";
  // incremented inside loop
  for (auto iter = voters.begin(); iter != voters.end();) {
    // decrement and test position
    if (iter->second->getMap().test(--iter->first)) {
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

void ConsensusBuilder::makeConsensus(BinaryMap *lastMap) {
  BinaryMap::Bitset contig;

  // voters for consensus
  std::list<std::pair<std::size_t, BinaryMap *>> voters;
  // add first voter
  voters.emplace_back(lastMap->getMap().size(), lastMap);
  // set counter to next voter
  std::size_t dist = 0;
  dist             = lastMap->getLeftNeighbour().second;

  // NOTE : shift is realative to what ?
  //        -> find all L_neighbours as it's mutual relation

  // run till no voter left
  while (!voters.empty()) {

    if (dist > 0) {
      // decrement distance to next voter
      --dist;
    } else {
      // add next map 'voter'
      while (dist == 0 && lastMap->getLeftNeighbour().first) {
        dist    = lastMap->getLeftNeighbour().second;
        lastMap = lastMap->getLeftNeighbour().first;
        voters.emplace_back(lastMap->getMap().size(), lastMap);
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

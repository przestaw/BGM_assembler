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

void ConsensusBuilder::runSingleConsensus() {
  detectContigs(); // TODO : implement and use

  // NOTE: temporary way of iterate over "contigs"
  for (auto &consensusMap : connectedMaps) {
    if (consensusMap.getRightNeighbour().first == nullptr) {
      makeConsensus(&consensusMap);
      std::cout << '\n' << consensusMap.getIdentifier() << '\n';
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

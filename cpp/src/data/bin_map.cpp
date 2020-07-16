#include <data/bin_map.h>

using namespace oma;

BinaryMap::BinaryMap(const std::string &c_identifier, Bitset c_map)
    : identifier(c_identifier), map(std::move(c_map)) {}

void BinaryMap::setLeftBest(BinaryMap *best, unsigned int shift) {
  leftNeighbour = std::make_pair(best, shift);
}

void BinaryMap::setRightBest(BinaryMap *best, unsigned int shift) {
  rightNeighbour = std::make_pair(best, shift);
}

const BinaryMap::Bitset &BinaryMap::getMap() const { return map; }

BinaryMap::Bitset BinaryMap::getMap() { return Bitset(map); }

const std::string &BinaryMap::getIdentifier() const { return identifier; }

const BinaryMap::Neighbour &oma::BinaryMap::getLeftNeighbour() const {
  return leftNeighbour;
}

const BinaryMap::Neighbour &oma::BinaryMap::getRightNeighbour() const {
  return rightNeighbour;
}

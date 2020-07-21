#include <data/bin_map.h>

using namespace oma;

BinaryMap::BinaryMap(const std::string &c_identifier, Bitset c_map)
    : identifier(c_identifier), map(std::move(c_map)),
      leftNeighbour(nullptr, 0), rightNeighbour(nullptr, 0) {}

void BinaryMap::setLeftNeighbour(BinaryMap *best, unsigned int shift) {
  leftNeighbour = std::make_pair(best, shift);
}

void BinaryMap::setRightNeighbour(BinaryMap *best, unsigned int shift) {
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

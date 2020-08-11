#include <data/bin_map.h>
#include <utility>

using namespace oma;

BinaryMap::BinaryMap(std::string c_identifier, Bitset c_map)
    : identifier(std::move(c_identifier)), map(std::move(c_map)),
      leftNeighbour(-1, 0), rightNeighbour(-1, 0) {}

void BinaryMap::setLeftNeighbour(int best, unsigned int shift) {
  leftNeighbour = std::make_pair(best, shift);
}

void BinaryMap::setRightNeighbour(int best, unsigned int shift) {
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

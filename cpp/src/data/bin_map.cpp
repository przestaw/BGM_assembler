#include <data/bin_map.h>

using namespace oma::data;

BinaryMap::BinaryMap(const std::string &c_identifier, Bitset c_map)
    : identifier(c_identifier), map(std::move(c_map)) {}

void BinaryMap::setLeftBest(BinaryMap *best, unsigned int shift) {
  lBest  = best;
  lShift = shift;
}

void BinaryMap::setRightBest(BinaryMap *best, unsigned int shift) {
  rBest  = best;
  rShift = shift;
}

BinaryMap::Bitset BinaryMap::getMap() { return Bitset(map); }

const std::string &BinaryMap::getIdentifier() const { return identifier; }

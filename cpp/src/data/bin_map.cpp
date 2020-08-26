#include <data/bin_map.h>
#include <utility>

using namespace oma;

BinaryMap::BinaryMap(std::string c_identifier, Bitset c_map)
    : identifier(std::move(c_identifier)), map(std::move(c_map)),
      leftNeighbour(-1, std::numeric_limits<unsigned>::max()),
      rightNeighbour(-1, std::numeric_limits<unsigned>::max()),
      mark(false)
    {}

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

std::string BinaryMap::toFastaRecord() const {
  std::string ret = "> ";
  ret += identifier;
  ret += "\n";

  // bitset string is reversed
  std::string temp;
  boost::to_string(map, temp);
  std::reverse(temp.begin(), temp.end());

  // write in 80 bits chunks
  unsigned count = 1 + (temp.size() - 1) / 80;
  for (unsigned i = 0; i < count; ++i) {
    ret += temp.substr(i * 80, 80) + "\n";
  }

  return ret;
}

//bool BinaryMap::isAligned() const {
//  return aligned;
//}
//
//void BinaryMap::setAligned(bool Aligned) {
//  aligned = Aligned;
//}

bool BinaryMap::isMark() const {
  return mark;
}

void BinaryMap::setMark(bool Mark) {
  mark = Mark;
}

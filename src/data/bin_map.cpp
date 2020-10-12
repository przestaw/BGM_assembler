#include <data/bin_map.h>
#include <utility>

using namespace oma;

BinaryMap::BinaryMap(std::string c_identifier, Bitset c_map)
    : identifier_(std::move(c_identifier)), map_(std::move(c_map)),
      leftNeighbour_(-1, std::numeric_limits<unsigned>::max()),
      rightNeighbour_(-1, std::numeric_limits<unsigned>::max()), mark_(false),
      contig_(false) {}

void BinaryMap::setLeftNeighbour(int best, unsigned int shift) {
  leftNeighbour_ = Neighbour(best, shift);
}

void BinaryMap::setRightNeighbour(int best, unsigned int shift) {
  rightNeighbour_ = Neighbour(best, shift);
}

const BinaryMap::Bitset &BinaryMap::getMap() const { return map_; }

BinaryMap::Bitset BinaryMap::getMap() { return Bitset(map_); }

const std::string &BinaryMap::getIdentifier() const { return identifier_; }

const BinaryMap::Neighbour &oma::BinaryMap::getLeftNeighbour() const {
  return leftNeighbour_;
}

const BinaryMap::Neighbour &oma::BinaryMap::getRightNeighbour() const {
  return rightNeighbour_;
}

std::string BinaryMap::toFastaRecord() const {
  std::string ret = "> ";
  ret += identifier_;
  ret += "\n";

  // bitset string is reversed
  std::string temp;
  boost::to_string(map_, temp);
  std::reverse(temp.begin(), temp.end());

  // write in 80 bits chunks
  unsigned count = 1 + (temp.size() - 1) / 80;
  for (unsigned i = 0; i < count; ++i) {
    ret += temp.substr(i * 80, 80) + "\n";
  }

  return ret;
}

bool BinaryMap::isMark() const { return mark_; }

void BinaryMap::setMark(bool Mark) { mark_ = Mark; }

bool BinaryMap::isContig() const { return contig_; }

void BinaryMap::setContig() { contig_ = true; }

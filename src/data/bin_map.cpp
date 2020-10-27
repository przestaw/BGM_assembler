#include <data/bin_map.h>
#include <utility>

using namespace bgm;

BinaryMap::BinaryMap(std::string c_identifier, std::string c_bmap)
    : identifier_(std::move(c_identifier)),
      leftNeighbour_(-1, std::numeric_limits<unsigned>::max()),
      rightNeighbour_(-1, std::numeric_limits<unsigned>::max()), mark_(false),
      contig_(false) {
  std::reverse(c_bmap.begin(), c_bmap.end());
  map_ = Bitset(c_bmap);
}

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

std::string BinaryMap::getPyIdentifier() const { return identifier_; }

std::string BinaryMap::toStringBMap() const {
  std::string ret;
  // bitset string is reversed
  boost::to_string(map_, ret);
  std::reverse(ret.begin(), ret.end());
  return ret;
}

void BinaryMap::setIdentifier(const std::string &identifier) { identifier_ = identifier; }

const BinaryMap::Neighbour &bgm::BinaryMap::getLeftNeighbour() const {
  return leftNeighbour_;
}

const BinaryMap::Neighbour &bgm::BinaryMap::getRightNeighbour() const {
  return rightNeighbour_;
}

std::string BinaryMap::toFastaRecord() const {
  // typical FASTA sequenc width
  const size_t width = 80;
  // header line
  std::string ret = "> ";
  ret += identifier_;
  ret += "\n";

  // bitset string is reversed - handled in toStringBMap
  std::string temp = toStringBMap();

  // write in 80 bits chunks
  unsigned count = 1 + (temp.size() - 1) / width;
  for (unsigned i = 0; i < count; ++i) { ret += temp.substr(i * width, width) + "\n"; }

  return ret;
}

bool BinaryMap::isMark() const { return mark_; }

void BinaryMap::setMark(bool mark) { mark_ = mark; }

bool BinaryMap::isContig() const { return contig_; }

void BinaryMap::setContig() { contig_ = true; }

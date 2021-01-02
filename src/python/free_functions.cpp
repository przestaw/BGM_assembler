#include <algorithms/align_algorithm.h>
#include <error/exeception.h>
#include <python/free_functions.h>

namespace p  = boost::python;
namespace np = boost::python::numpy;

unsigned bgm::py::xorCountBinaryMaps(bgm::BinaryMap l, bgm::BinaryMap r) {
  if (l.getMap().size() != r.getMap().size()) {
    throw bgm::error::MismatchedArgumentsSize();
  }

  bgm::BinaryMap::Bitset lMap = l.getMap();
  lMap ^= r.getMap();
  return lMap.count();
}

bgm::BinaryMap bgm::py::xorBinaryMaps(bgm::BinaryMap l, bgm::BinaryMap r) {
  if (l.getMap().size() != r.getMap().size()) {
    throw bgm::error::MismatchedArgumentsSize();
  }

  bgm::BinaryMap::Bitset lMap = l.getMap();
  lMap ^= r.getMap();

  return bgm::BinaryMap("XOR " + l.getIdentifier() + " & " + r.getIdentifier(), lMap);
}

int bgm::py::findBestShift(bgm::BinaryMap l, bgm::BinaryMap r) {
  return bgm::AlignAlgorithm::pyAlignFromBegins(l.getMap(), r.getMap()).getShift();
}

p::tuple bgm::py::findBestAlignemnt(bgm::BinaryMap l, bgm::BinaryMap r) {
  const auto res = bgm::AlignAlgorithm::pyAlignFromBegins(l.getMap(), r.getMap());
  return p::make_tuple(res.getDiffrences(), res.getShift(), res.getOverlap());
}

size_t bgm::py::lengthOfBinaryMap(const bgm::BinaryMap& map) {
  return map.getMap().size();
}

bgm::BinaryMap bgm::py::copyBinaryMap(const bgm::BinaryMap& map) {
  return bgm::BinaryMap(map.getIdentifier(), map.getMap());
}

bgm::BinaryMap bgm::py::truncate(bgm::BinaryMap& map, size_t size) {
  auto bmap = map.getMap();
  if (map.getMap().size() > size) {
    bmap.resize(size);
  }
  return bgm::BinaryMap(map.getIdentifier(), bmap);
}

bgm::BinaryMap bgm::py::shiftLeft(bgm::BinaryMap& map, size_t shift) {
  auto bmap = map.getMap();
  if (bmap.size() > shift) {
    bmap = bmap >> shift;
    bmap.resize(bmap.size() - shift);
  } else {
    bmap.resize(0);
  }
  return bgm::BinaryMap(map.getIdentifier(), bmap);
}

#include <algorithms/align_algorithm.h>
#include <python_functions.h>

unsigned bgm::xorBinaryMaps(bgm::BinaryMap l, bgm::BinaryMap r) {
  if (l.getMap().size() != r.getMap().size()) {
    throw std::runtime_error("Mismatched arguments size");
  }

  bgm::BinaryMap::Bitset lMap = l.getMap();
  lMap ^= r.getMap();
  return lMap.count();
}

unsigned bgm::findBestAlignemnt(bgm::BinaryMap l, bgm::BinaryMap r) {
  return bgm::AlignAlgorithm::pyAlignFromBegins(l.getMap(), r.getMap());
}

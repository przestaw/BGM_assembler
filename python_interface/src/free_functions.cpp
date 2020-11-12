#include <algorithms/align_algorithm.h>
#include <free_functions.h>

namespace p  = boost::python;
namespace np = boost::python::numpy;

unsigned bgm::py::xorCountBinaryMaps(bgm::BinaryMap l, bgm::BinaryMap r) {
  if (l.getMap().size() != r.getMap().size()) {
    throw std::runtime_error("Mismatched arguments size");
  }

  bgm::BinaryMap::Bitset lMap = l.getMap();
  lMap ^= r.getMap();
  return lMap.count();
}

bgm::BinaryMap bgm::py::xorBinaryMaps(bgm::BinaryMap l, bgm::BinaryMap r) {
  if (l.getMap().size() != r.getMap().size()) {
    throw std::runtime_error("Mismatched arguments size");
  }

  bgm::BinaryMap::Bitset lMap = l.getMap();
  lMap ^= r.getMap();

  return bgm::BinaryMap("XOR " + l.getIdentifier() + " & " + r.getIdentifier(), lMap);
}

int bgm::py::findBestAlignemnt(bgm::BinaryMap l, bgm::BinaryMap r) {
  return bgm::AlignAlgorithm::pyAlignFromBegins(l.getMap(), r.getMap()).getShift();
}

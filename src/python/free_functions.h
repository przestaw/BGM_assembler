//
// Created by przestaw on 06.11.2020.
//

#ifndef BGM_ASM_FREE_FUNCTIONS_H
#define BGM_ASM_FREE_FUNCTIONS_H

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <data/bin_map.h>

namespace bgm::py {
  unsigned xorCountBinaryMaps(bgm::BinaryMap l, bgm::BinaryMap r);

  bgm::BinaryMap xorBinaryMaps(bgm::BinaryMap l, bgm::BinaryMap r);

  int findBestShift(bgm::BinaryMap l, bgm::BinaryMap r);

  boost::python::tuple findBestAlignemnt(bgm::BinaryMap l, bgm::BinaryMap r);

  size_t lengthOfBinaryMap(const bgm::BinaryMap &map);

  bgm::BinaryMap copyBinaryMap(const bgm::BinaryMap &map);

  bgm::BinaryMap truncate(bgm::BinaryMap &map, size_t size);

  bgm::BinaryMap shiftLeft(bgm::BinaryMap &map, size_t shift);

} // namespace bgm::py

#endif // BGM_ASM_FREE_FUNCTIONS_H

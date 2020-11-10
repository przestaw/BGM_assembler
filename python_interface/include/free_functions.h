//
// Created by przestaw on 06.11.2020.
//

#ifndef BGM_ASM_FREE_FUNCTIONS_H
#define BGM_ASM_FREE_FUNCTIONS_H

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <data/bin_map.h>

namespace bgm::py {
  unsigned xorBinaryMaps(bgm::BinaryMap l, bgm::BinaryMap r);

  int findBestAlignemnt(bgm::BinaryMap l, bgm::BinaryMap r);
} // namespace bgm::py

#endif // BGM_ASM_FREE_FUNCTIONS_H

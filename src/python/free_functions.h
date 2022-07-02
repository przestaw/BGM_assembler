//
// Created by przestaw on 06.11.2020.
//

#ifndef BGM_ASM_FREE_FUNCTIONS_H
#define BGM_ASM_FREE_FUNCTIONS_H

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <data/bin_map.h>

namespace bgm::py {
  /**
   * Counts differing bits (XOR population count) between two binary maps of equal size
   * @param l first binary map
   * @param r second binary map
   * @return number of differing positions
   */
  unsigned xorCountBinaryMaps(bgm::BinaryMap l, bgm::BinaryMap r);

  /**
   * Computes bitwise XOR between two binary maps of equal size
   * @param l first binary map
   * @param r second binary map
   * @return resulting XOR BinaryMap
   */
  bgm::BinaryMap xorBinaryMaps(bgm::BinaryMap l, bgm::BinaryMap r);

  /**
   * Finds shift offset yielding the minimum number of differences between two maps
   * @param l reference binary map
   * @param r tested binary map
   * @return best shift offset from map beginnings
   */
  int findBestShift(bgm::BinaryMap l, bgm::BinaryMap r);

  /**
   * Finds best alignment between two binary maps
   * @param l reference binary map
   * @param r tested binary map
   * @return Python tuple (differences, shift, overlap)
   */
  boost::python::tuple findBestAlignemnt(bgm::BinaryMap l, bgm::BinaryMap r);

  /**
   * Returns length of a binary map in quants
   * @param map binary map
   * @return number of bits in map
   */
  size_t lengthOfBinaryMap(const bgm::BinaryMap &map);

  /**
   * Creates a deep copy of a binary map
   * @param map binary map to copy
   * @return copied BinaryMap
   */
  bgm::BinaryMap copyBinaryMap(const bgm::BinaryMap &map);

  /**
   * Truncates binary map to given size if current length exceeds it
   * @param map binary map to truncate
   * @param size target maximum length
   * @return truncated copy of BinaryMap
   */
  bgm::BinaryMap truncate(bgm::BinaryMap &map, size_t size);

  /**
   * Shifts binary map to the left by discarding leading bits
   * @param map binary map to shift
   * @param shift number of bits to discard from the beginning
   * @return shifted copy of BinaryMap
   */
  bgm::BinaryMap shiftLeft(bgm::BinaryMap &map, size_t shift);

} // namespace bgm::py

#endif // BGM_ASM_FREE_FUNCTIONS_H

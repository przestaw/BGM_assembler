//
// Created by przestaw on 10.11.2020.
//

#ifndef BGM_ASM_MARKER_TYPE_H
#define BGM_ASM_MARKER_TYPE_H

#include <cstdint>

namespace bgm::py {
  /**
   * Marker types
   */
  enum struct MarkerType : int8_t {
    Empty                 = -1, //!< represents no map
    Map                   = 0,  //!< represents binary map with no marker
    Marker                = 1,  //!< represents marker on map or comparison
    CompareOrReferenceMap = 2   //!< represents comparison line or reference map
  };

  /**
   * Maps a MarkerType enum value to a user-provided numeric representation
   * @tparam ValueType output numeric type
   * @param val marker type to convert
   * @param empty value returned for MarkerType::Empty
   * @param marker value returned for MarkerType::Marker
   * @param map value returned for MarkerType::Map
   * @param compare value returned for MarkerType::CompareOrReferenceMap
   * @return corresponding numeric value
   */
  template <typename ValueType>
  inline ValueType markerToValueType(MarkerType val,
                                     ValueType empty,
                                     ValueType marker,
                                     ValueType map,
                                     ValueType compare) {
    switch (val) {
    case MarkerType::Marker:
      return marker;
    default: // to silence GCC warning
    case MarkerType::Empty:
      return empty;
    case MarkerType::Map:
      return map;
    case MarkerType::CompareOrReferenceMap:
      return compare;
    }
  }

  /**
   * Returns MarkerType at position n for a shifted query binary map
   * @param n global column position
   * @param start start offset of the binary map
   * @param bMap binary map
   * @return MarkerType at position n
   */
  inline MarkerType
  getMapValAtPos(const int n, const int start, const bgm::BinaryMap& bMap) {
    if (n < start) {
      return MarkerType::Empty;
    } else if (n < bMap.getMap().size() + start) {
      return bMap.getMap().test(n - start) ? MarkerType::Marker : MarkerType::Map;
    } else {
      return MarkerType::Empty;
    }
  }

  /**
   * Returns MarkerType at position n for a shifted reference binary map
   * @param n global column position
   * @param start start offset of the reference map
   * @param bMap reference binary map
   * @return MarkerType at position n
   */
  inline MarkerType
  getRefValAtPos(const int n, const int start, const bgm::BinaryMap& bMap) {
    if (n < start) {
      return MarkerType::Empty;
    } else if (n < bMap.getMap().size() + start) {
      return bMap.getMap().test(n - start) ? MarkerType::Marker
                                           : MarkerType::CompareOrReferenceMap;
    } else {
      return MarkerType::Empty;
    }
  }
} // namespace bgm::py

#endif // BGM_ASM_MARKER_TYPE_H

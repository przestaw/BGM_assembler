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

  inline MarkerType
  getMapValAtPos(const size_t n, const size_t start, const bgm::BinaryMap& bMap) {
    if (n < start) {
      return MarkerType::Empty;
    } else if (n < bMap.getMap().size() + start) {
      return bMap.getMap().test(n - start) ? MarkerType::Marker : MarkerType::Map;
    } else {
      return MarkerType::Empty;
    }
  }

  inline MarkerType
  getRefValAtPos(const size_t n, const size_t start, const bgm::BinaryMap& bMap) {
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

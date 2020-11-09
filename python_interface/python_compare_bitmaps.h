//
// Created by przestaw on 09.11.2020.
//

#ifndef BGM_ASM_PYTHON_COMPARE_BITMAPS_H
#define BGM_ASM_PYTHON_COMPARE_BITMAPS_H

#include <algorithms/align_algorithm.h>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <data/bin_map.h>

#include <iostream>

namespace bgm {
  template <typename ValueType>
  boost::python::numpy::ndarray alignMapToRef(bgm::BinaryMap l,
                                              bgm::BinaryMap r,
                                              ValueType none,
                                              ValueType label,
                                              ValueType map,
                                              ValueType compare) {

    const auto align  = bgm::AlignAlgorithm::pyAlignFromBegins(l.getMap(), r.getMap());
    const size_t lLen = l.getMap().size();
    const size_t rLen = r.getMap().size();

    const size_t totalLen =
        (l.getMap().size() + r.getMap().size()) - align.getOverlap();

    const size_t startL = align.getShift() < 0 ? -align.getShift() : 0;
    const size_t startR = align.getShift() > 0 ? +align.getShift() : 0;

    auto getValAtPos =
        [&](const size_t n, const size_t start, const bgm::BinaryMap& bMap) -> int8_t {
      if (n < start) {
        return -1;
      } else if (n < bMap.getMap().size() + start) {
        return static_cast<int8_t>(bMap.getMap().test(n - start));
      } else {
        return -1;
      }
    };

    auto getValFromInt = [&](const int8_t i) -> ValueType {
      if (i < 0) {
        return none;
      } else if (i == 0) {
        return map;
      } else {
        return label;
      }
    };

    auto getCmpValFromInt = [&](const int8_t i) -> ValueType {
      if (i < 0) {
        return none;
      } else if (i == 0) {
        return compare;
      } else {
        return label;
      }
    };

    auto data = std::vector<int8_t>(3 * totalLen);

    for (auto i = 0; i < totalLen; ++i) {
      const auto lVal = getValAtPos(i, startL, l);
      const auto rVal = getValAtPos(i, startR, r);

      data[i]            = getValFromInt(lVal);
      data[i + totalLen] = getValFromInt(rVal);

      const auto xorVal =
          (lVal >= 0 && rVal >= 0) ? static_cast<int8_t>(lVal xor rVal) : -1;

      data[i + 2 * totalLen] = getCmpValFromInt(xorVal);
    }

    // NOTE: need to copy data to maintain ownership !
    boost::python::numpy::ndarray ret = boost::python::numpy::zeros(
        boost::python::make_tuple(3, totalLen),
        boost::python::numpy::dtype::get_builtin<ValueType>());
    std::copy(data.begin(), data.end(), reinterpret_cast<ValueType*>(ret.get_data()));

    return ret;
  }
} // namespace bgm

#endif // BGM_ASM_PYTHON_COMPARE_BITMAPS_H

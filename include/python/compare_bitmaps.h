//
// Created by przestaw on 09.11.2020.
//

#ifndef BGM_ASM_COMPARE_BITMAPS_H
#define BGM_ASM_COMPARE_BITMAPS_H

#include <algorithms/align_algorithm.h>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <data/bin_map.h>
#include <marker_type.h>

namespace bgm::py {

  class CompareBitmaps {
  public:
    using MarkerType = bgm::py::MarkerType;
    /**
     * Constructos
     * @param l reference "left" or upper map
     * @param r tested "right" or lower map
     */
    CompareBitmaps(bgm::BinaryMap l, bgm::BinaryMap r);

    /**
     * Method performing comparison between two maps
     * @return linear vector of 3 lines of comparison bitmap
     */
    std::vector<MarkerType> alignMapToRef();

    /**
     * Method performing comparison between two maps,
     * result is 3 line numpy array with aligned maps and overlap
     * @tparam ValueType type of values in returned array
     * @param empty empty value
     * @param marker marker value
     * @param map map value
     * @param compare compare value
     * @return numpy array with comparison
     */
    template <typename ValueType>
    boost::python::numpy::ndarray
    alignMapToRefPy(ValueType empty, ValueType marker, ValueType map, ValueType compare);

  private:
    bgm::BinaryMap l_;
    bgm::BinaryMap r_;
  };

  template <typename ValueType>
  boost::python::numpy::ndarray CompareBitmaps::alignMapToRefPy(ValueType empty,
                                                                ValueType marker,
                                                                ValueType map,
                                                                ValueType compare) {
    const auto converter = [&](MarkerType val) {
      return markerToValueType(val, empty, marker, map, compare);
    };

    const auto data = this->alignMapToRef();

    // NOTE: need to copy data to maintain ownership !
    boost::python::numpy::ndarray ret = boost::python::numpy::zeros(
        boost::python::make_tuple(3, data.size() / 3),
        boost::python::numpy::dtype::get_builtin<ValueType>());
    // std::copy(data.begin(), data.end(), reinterpret_cast<ValueType*>(ret.get_data()));
    std::transform(data.begin(),
                   data.end(),
                   reinterpret_cast<ValueType*>(ret.get_data()),
                   converter);

    return ret;
  }

} // namespace bgm::py

#endif // BGM_ASM_COMPARE_BITMAPS_H

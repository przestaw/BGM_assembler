//
// Created by przestaw on 10.11.2020.
//

#ifndef BGM_ASM_MULTI_COMPARE_BITMAPS_H
#define BGM_ASM_MULTI_COMPARE_BITMAPS_H

#include <boost/python/numpy.hpp>
#include <data/bin_map.h>
#include <marker_type.h>
#include <mutex>

namespace bgm::py{
  class MultiCompareBitmaps {
  public:
    using MarkerType = bgm::py::MarkerType;

    MultiCompareBitmaps(bgm::BinaryMap  ref);

    void addMap(bgm::BinaryMap map);

    /**
     * Method performing comparison between maps
     * @return linear vector of lines of aligned maps bitmap
     */
    std::vector<MarkerType> alignMapToRef();

    /**
     * Method performing comparison between two maps,
     * result is multiline line numpy array with aligned maps
     * @tparam ValueType type of values in returned array
     * @param empty empty value
     * @param marker marker value
     * @param map map value
     * @param refmap compare value
     * @return numpy array with comparison
     */
    template <typename ValueType>
    boost::python::numpy::ndarray
    alignMapToRefPy(ValueType empty, ValueType marker, ValueType map, ValueType refmap);
  private:
    mutable std::mutex mapsGuard;
    bgm::BinaryMap ref_;
    std::vector<bgm::BinaryMap> maps_;
  };

  template <typename ValueType>
  boost::python::numpy::ndarray MultiCompareBitmaps::alignMapToRefPy(ValueType empty,
                                                                     ValueType marker,
                                                                     ValueType map,
                                                                     ValueType refmap) {
    const auto converter = [&](MarkerType val) {
      return markerToValueType(val, empty, marker, map, refmap);
    };

    // TODO -> change type to vec<vec<value>>
    const auto data = this->alignMapToRef();

    // NOTE: need to copy data to maintain ownership !
    boost::python::numpy::ndarray ret = boost::python::numpy::zeros(
        // TODO : shape == (lines, size/lines)
        boost::python::make_tuple(1, data.size()),
        boost::python::numpy::dtype::get_builtin<ValueType>());
    std::transform(data.begin(),
                   data.end(),
                   reinterpret_cast<ValueType*>(ret.get_data()),
                   converter);

    return ret;
  }
}
#endif // BGM_ASM_MULTI_COMPARE_BITMAPS_H

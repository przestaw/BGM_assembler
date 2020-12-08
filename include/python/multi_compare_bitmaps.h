//
// Created by przestaw on 10.11.2020.
//

#ifndef BGM_ASM_MULTI_COMPARE_BITMAPS_H
#define BGM_ASM_MULTI_COMPARE_BITMAPS_H

#include <boost/python/numpy.hpp>
#include <data/bin_map.h>
#include <marker_type.h>
#include <mutex>

namespace bgm::py {
  class MultiCompareBitmaps {
  public:
    using MarkerType = bgm::py::MarkerType;

    MultiCompareBitmaps(bgm::BinaryMap ref);

    void addMap(bgm::BinaryMap map);

    /**
     * Method performing comparison between maps
     * @return linear vector of lines of aligned maps bitmap
     */
    std::vector<MarkerType> alignMapsToRef();

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

    /**
     * Returns names of maps in comparison as python list
     * @return names of maps in comparison
     */
    boost::python::list getNames();

  private:
    // input data
    mutable std::mutex mapsGuard;
    bgm::BinaryMap ref_;
    std::vector<bgm::BinaryMap> maps_;

    // struct for data save
    struct MapAlignDescriptor {
      MapAlignDescriptor(const BinaryMap& ref,
                         int shift      = 0,
                         size_t overlap = 0,
                         size_t fo      = 0,
                         size_t f       = 0,
                         size_t r       = 0,
                         size_t ro      = 0);
      const bgm::BinaryMap& ref;
      int shift;
      size_t overlap;
      size_t fo;
      size_t f;
      size_t r;
      size_t ro;
    };

    // internal stages data
    bool aligned = false;
    size_t maxFo = 0;
    size_t maxRo = 0;
    std::vector<MapAlignDescriptor> alignResults;

    // output data
    std::vector<MarkerType> adjusted;
    std::vector<std::string> names;

    // stages
    void alignMaps();
    void adjustAndElongate();
    void adjustAndTrucnate();
  };

  template <typename ValueType>
  boost::python::numpy::ndarray MultiCompareBitmaps::alignMapToRefPy(ValueType empty,
                                                                     ValueType marker,
                                                                     ValueType map,
                                                                     ValueType refmap) {
    const auto converter = [&](MarkerType val) {
      return markerToValueType(val, empty, marker, map, refmap);
    };

    alignMapsToRef();

    // NOTE: need to copy data to maintain ownership !
    boost::python::numpy::ndarray ret = boost::python::numpy::zeros(
        // TODO : shape == (lines, size/lines)
        boost::python::make_tuple(names.size(), ref_.getMap().size()),
        boost::python::numpy::dtype::get_builtin<ValueType>());
    std::transform(adjusted.begin(),
                   adjusted.end(),
                   reinterpret_cast<ValueType*>(ret.get_data()),
                   converter);

    return ret;
  }
} // namespace bgm::py
#endif // BGM_ASM_MULTI_COMPARE_BITMAPS_H

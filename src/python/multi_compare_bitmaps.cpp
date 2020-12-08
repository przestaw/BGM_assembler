//
// Created by przestaw on 10.11.2020.
//
#include <algorithms/align_algorithm.h>
#include <error/exeception.h>
#include <multi_compare_bitmaps.h>
#include <utility>

bgm::py::MultiCompareBitmaps::MultiCompareBitmaps(bgm::BinaryMap ref)
    : ref_(std::move(ref)) {}

void bgm::py::MultiCompareBitmaps::addMap(bgm::BinaryMap map) {
  // lock to do not interfere with execution
  std::unique_lock lock(mapsGuard);
  maps_.push_back(std::move(map));
}

std::vector<bgm::py::MultiCompareBitmaps::MarkerType>
bgm::py::MultiCompareBitmaps::alignMapsToRef() {
  // lock to prevent map addition
  std::unique_lock lock(mapsGuard);

  this->alignMaps();

  this->adjustAndTrucnate();
  /*
    # perform adjustments
        for map_iter, data in zip(others, self.map_data):
            map_str = map_iter.string_map

            if truncate:
                if data.fo > 0:
                    map_str = map_str[data.fo:]
                elif data.f > 0:
                    map_str = data.f * 'n' + map_str

                if data.ro > 0:
                    map_str = map_str[:-data.ro]
                elif data.r > 0:
                    map_str = map_str + data.r * 'n'
            else:
                if data.fo > 0:
                    map_str = (max_fo - data.fo) * 'n' + map_str
                elif data.f > 0:
                    map_str = (max_fo + data.f) * 'n' + map_str
                else:
                    map_str = max_fo * 'n' + map_str

                if data.ro > 0:
                    map_str = map_str + (max_ro - data.ro) * 'n'
                elif data.r > 0:
                    map_str = map_str + (max_ro + data.r) * 'n'
                else:
                    map_str = map_str + max_ro * 'n'

            self.maps.append(map_str)

        if not truncate:
            self.ref = max_fo * 'n' + self.ref + max_ro * 'n'
   */

  return adjusted;
}

bgm::py::MultiCompareBitmaps::MapAlignDescriptor::MapAlignDescriptor(
    const bgm::BinaryMap& ref,
    int shift,
    size_t overlap,
    size_t fo,
    size_t f,
    size_t r,
    size_t ro)
    : ref(ref), shift(shift), overlap(overlap), fo(fo), f(f), r(r), ro(ro) {}

void bgm::py::MultiCompareBitmaps::alignMaps() {
  const size_t refLen = ref_.getMap().size();

  for (const auto& map : maps_) {
    const auto result =
        bgm::AlignAlgorithm::pyAlignFromBegins(ref_.getMap(), map.getMap());

    MapAlignDescriptor pivot(map);

    if (result.getShift() > 0) {
      pivot.f         = static_cast<size_t>(result.getShift());
      const int tempR = pivot.f + refLen - map.getMap().size();
      if (tempR < 0) {
        pivot.ro = static_cast<size_t>(-tempR);
      } else {
        pivot.r = static_cast<size_t>(tempR);
      }
    } else {
      pivot.fo        = static_cast<size_t>(-result.getShift());
      const int tempR = pivot.fo - refLen + map.getMap().size();
      if (tempR < 0) {
        pivot.ro = static_cast<size_t>(-tempR);
      } else {
        pivot.r = static_cast<size_t>(tempR);
      }
    }

    maxFo = maxFo > pivot.fo ? maxFo : pivot.fo;
    maxRo = maxRo > pivot.ro ? maxRo : pivot.ro;

    alignResults.push_back(std::move(pivot));
  }
}

void bgm::py::MultiCompareBitmaps::adjustAndElongate() {
  throw bgm::error::RuntimeException("Not Implemented");
}

void bgm::py::MultiCompareBitmaps::adjustAndTrucnate() {
  for (size_t i = 0; i < ref_.getMap().size(); ++i) {
    this->adjusted.push_back(bgm::py::getRefValAtPos(i, 0, ref_));
  }
  this->names.emplace_back(ref_.getIdentifier());

  for (const auto& pivot : alignResults) {
    const int start = pivot.f > 0 ? pivot.f : -pivot.fo;
    for (size_t i = 0; i < ref_.getMap().size(); ++i) {
      this->adjusted.push_back(bgm::py::getMapValAtPos(i, start, pivot.ref));
    }
    this->names.emplace_back(pivot.ref.getIdentifier());
  }
}

boost::python::list bgm::py::MultiCompareBitmaps::getNames() {
  std::unique_lock lock(mapsGuard);
  // result must be returned as python list
  boost::python::list list;
  // reuse code from AssemblyTask
  for (const auto& map : names) { list.append(map); }
  return list;
}

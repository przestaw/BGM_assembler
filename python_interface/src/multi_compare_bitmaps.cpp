//
// Created by przestaw on 10.11.2020.
//
#include <algorithms/align_algorithm.h>
#include <multi_compare_bitmaps.h>
#include <utility>

bgm::py::MultiCompareBitmaps::MultiCompareBitmaps(bgm::BinaryMap ref)
    : ref_(std::move(ref)) {}

// bgm::py::MultiCompareBitmaps::MapAlignDescriptor::MapAlignDescriptor(std::string
// name_c,
//                                                                     int shift_c,
//                                                                     size_t overlap_c,
//                                                                     size_t fo_c,
//                                                                     size_t f_c,
//                                                                     size_t r_c,
//                                                                     size_t ro_c)
//    : name(std::move(name_c)), shift(shift_c), overlap(overlap_c), fo(fo_c), f(f_c),
//      r(r_c), ro(ro_c) {}

void bgm::py::MultiCompareBitmaps::addMap(bgm::BinaryMap map) {
  // lock to do not interfere with execution
  std::unique_lock lock(mapsGuard);
  maps_.push_back(std::move(map));
}

std::vector<bgm::py::MultiCompareBitmaps::MarkerType>
bgm::py::MultiCompareBitmaps::alignMapsToRef() {
  // lock to prevent map addition
  std::unique_lock lock(mapsGuard);
  std::vector<MapAlignDescriptor> alignResults;

  const size_t refLen = ref_.getMap().size();
  size_t maxFo        = 0;
  size_t maxRo        = 0;

  for (const auto& map : maps_) {
    const auto result =
        bgm::AlignAlgorithm::pyAlignFromBegins(ref_.getMap(), map.getMap());

    MapAlignDescriptor pivot;

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

    pivot.name = std::to_string(result.getQuality()) + " : " + map.getIdentifier();

    alignResults.push_back(std::move(pivot));
  }

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

  return std::vector<MarkerType>();
}
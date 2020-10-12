//
// Created by przestaw on 12.10.2020.
//

#ifndef BGM_ASM_MAP_ALIGNMENT_H
#define BGM_ASM_MAP_ALIGNMENT_H

#include <cstdint>

namespace oma {
  class MapAlignment { // NOTE: WIP Alignment -> MapAlignment
  public:
    MapAlignment(int64_t map, size_t shift, double quality = 0.0)
        : map_(map), shift_(shift), quality_(quality) {}

    [[nodiscard]] int64_t getMap() const { return map_; }
    [[nodiscard]] size_t getShift() const { return shift_; }
    [[nodiscard]] double getQuality() const { return quality_; }

  private:
    const int64_t map_;
    const size_t shift_;
    const double quality_;
  };
} // namespace oma

#endif // BGM_ASM_MAP_ALIGNMENT_H

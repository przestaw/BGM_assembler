//
// Created by przestaw on 12.10.2020.
//

#ifndef BGM_ASM_MAP_ALIGNMENT_H
#define BGM_ASM_MAP_ALIGNMENT_H

#include <cstdint>

namespace bgm {
  /**
   * Class wrapping information about map alignment
   */
  class MapAlignment { // NOTE: WIP Alignment -> MapAlignment
  public:
    /**
     * Constructor
     * @param map map id
     * @param shift shift between maps
     * @param quality quality of alignment
     */
    MapAlignment(int64_t map, std::size_t shift, double quality = 0.0)
        : map_(map), shift_(shift), quality_(quality) {}

    /**
     * Getter for map id
     * @return map id
     */
    [[nodiscard]] int64_t getMap() const { return map_; }
    /**
     * Getter for shift between maps
     * @return shift between maps
     */
    [[nodiscard]] std::size_t getShift() const { return shift_; }
    /**
     * Getter for quality of alignment
     * @return quality of alignment
     */
    [[nodiscard]] double getQuality() const { return quality_; }

    inline bool operator<(const MapAlignment& rhs) const {
      return this->quality_ < rhs.quality_;
    }
    inline bool operator>(const MapAlignment& rhs) const { return rhs < (*this); }
    inline bool operator<=(const MapAlignment& rhs) const { return !((*this) > rhs); }
    inline bool operator>=(const MapAlignment& rhs) const { return !((*this) < rhs); }

  private:
    const int64_t map_;
    const std::size_t shift_;
    const double quality_;
  };
} // namespace bgm

#endif // BGM_ASM_MAP_ALIGNMENT_H

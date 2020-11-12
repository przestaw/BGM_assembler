//
// Created by przestaw on 12.10.2020.
//

#ifndef BGM_ASM_MAP_ALIGNMENT_H
#define BGM_ASM_MAP_ALIGNMENT_H

#include <cstdint>
#include <ostream>

namespace bgm {
  /**
   * Class wrapping information about map alignment
   */
  class MapAlignment { // NOTE: WIP Alignment -> MapAlignment, std::optional
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
    [[nodiscard]] inline int64_t getMap() const { return map_; }
    /**
     * Getter for shift between maps
     * @return shift between maps
     */
    [[nodiscard]] inline std::size_t getShift() const { return shift_; }
    /**
     * Getter for quality of alignment
     * @return quality of alignment
     */
    [[nodiscard]] inline double getQuality() const { return quality_; }

    inline bool operator<(const MapAlignment& rhs) const {
      return this->quality_ < rhs.quality_;
    }
    inline bool operator>(const MapAlignment& rhs) const { return rhs < (*this); }
    inline bool operator<=(const MapAlignment& rhs) const { return !((*this) > rhs); }
    inline bool operator>=(const MapAlignment& rhs) const { return !((*this) < rhs); }

  private:
    // data
    const int64_t map_;
    const std::size_t shift_;
    const double quality_;
  };

  class PyAlignment {
  public:
    PyAlignment(const unsigned long shift,
                const unsigned long overlap,
                const std::size_t quality)
        : shift_(shift), overlap_(overlap), diffrences_(quality) {}

    [[nodiscard]] inline int getShift() const { return shift_; }
    [[nodiscard]] inline std::size_t getOverlap() const { return overlap_; }
    [[nodiscard]] inline std::size_t getDiffrences() const { return diffrences_; }

    inline bool operator==(const PyAlignment& rhs) const {
      return this->shift_ == rhs.shift_ && this->overlap_ == rhs.overlap_
             && this->diffrences_ == rhs.diffrences_;
    }
    inline bool operator!=(const PyAlignment& rhs) const { return !(*this == rhs); }

    friend std::ostream& operator<<(std::ostream& out, const PyAlignment& alignment);

  private:
    const int shift_;
    const std::size_t overlap_;
    const std::size_t diffrences_;
  };

  inline std::ostream& operator<<(std::ostream& out, const PyAlignment& alignment) {
    out << "(" << alignment.getShift() << ", " << alignment.getOverlap() << ", "
        << alignment.getDiffrences() << ")";
    return out;
  }
} // namespace bgm

#endif // BGM_ASM_MAP_ALIGNMENT_H

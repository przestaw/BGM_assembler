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
  class LayoutAlignment { // NOTE: WIP Alignment -> MapAlignment, std::optional
  public:
    /**
     * Constructor
     * @param map map id
     * @param shift shift between maps
     * @param quality quality of alignment
     */
    LayoutAlignment(int64_t map, std::size_t shift, double quality = 0.0)
        : map_(map), shift_(shift), quality_(quality) {}

    /**
     * Mirror the alignment for second map
     * @param map second map
     * @return mirrored alignment
     */
    inline LayoutAlignment mirrorAlignment(int64_t map) {
      LayoutAlignment copy = *this;
      copy.map_            = map;
      return copy;
    }

    /**
     * Returns not an alignment [map index is negative]
     * @return not an alignment
     */
    static LayoutAlignment notAligned() {
      static LayoutAlignment notAlign =
          LayoutAlignment(-1, 0, std::numeric_limits<double>::max());
      return notAlign;
    }

    /**
     * Checks if alignment is valid [map index is positive]
     * @return true if alignment is valid
     */
    inline bool isValid() const { return map_ >= 0; }

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

    inline bool operator<(const LayoutAlignment& rhs) const {
      return this->quality_ < rhs.quality_;
    }
    inline bool operator>(const LayoutAlignment& rhs) const { return rhs < (*this); }
    inline bool operator<=(const LayoutAlignment& rhs) const { return !((*this) > rhs); }
    inline bool operator>=(const LayoutAlignment& rhs) const { return !((*this) < rhs); }

  private:
    // data
    int64_t map_;
    std::size_t shift_;
    double quality_;
  };

  class VisualAlignment {
  public:
    VisualAlignment(const unsigned long shift,
                    const unsigned long overlap,
                    const std::size_t quality)
        : shift_(shift), overlap_(overlap), diffrences_(quality) {}

    [[nodiscard]] inline int getShift() const { return shift_; }
    [[nodiscard]] inline std::size_t getOverlap() const { return overlap_; }
    [[nodiscard]] inline std::size_t getDiffrences() const { return diffrences_; }

    inline bool operator==(const VisualAlignment& rhs) const {
      return this->shift_ == rhs.shift_ && this->overlap_ == rhs.overlap_
             && this->diffrences_ == rhs.diffrences_;
    }
    inline bool operator!=(const VisualAlignment& rhs) const { return !(*this == rhs); }

    friend std::ostream& operator<<(std::ostream& out, const VisualAlignment& alignment);

  private:
    const int shift_;
    const std::size_t overlap_;
    const std::size_t diffrences_;
  };

  inline std::ostream& operator<<(std::ostream& out, const VisualAlignment& alignment) {
    out << "(" << alignment.getShift() << ", " << alignment.getOverlap() << ", "
        << alignment.getDiffrences() << ")";
    return out;
  }
} // namespace bgm

#endif // BGM_ASM_MAP_ALIGNMENT_H

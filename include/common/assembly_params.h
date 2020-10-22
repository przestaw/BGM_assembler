//
// Created by przestaw on 12.10.2020.
//

#ifndef BGM_ASM_ASSEMBLYPARAMS_H
#define BGM_ASM_ASSEMBLYPARAMS_H

#include <string>
namespace bgm {
  /**
   * Params for assembly task
   */
  class AssemblyParams {
  public:
    /**
     * Simplified default params constructor
     */
    AssemblyParams() : AssemblyParams(1, 200, 0.5, 0.1, 0, true, false) {}

    /**
     * Constructor
     * @param threads no of threads, 0 will enable all available on the
     * machine, other values will create demanded no of threads
     * @param minOverlap minimum overlap length
     * @param minPart minimum overlap part of smaller map
     * @param threshold threshold of differences for overlap to be meaningful
     * @param repeats no of folds to perform before output
     * @param markerPreference preference of marker or empty space
     * @param reuseMaps reuse all maps after each fold to pile up rmaps on
     * contigs
     */
    AssemblyParams(unsigned int threads,
                   unsigned int minOverlap,
                   double minPart,
                   double threshold,
                   unsigned repeats,
                   bool markerPreference,
                   bool reuseMaps)
        : threads_(threads), minOverlap_(minOverlap), minPart_(minPart),
          threshold_(threshold), repeats_(repeats), markerPreference_(markerPreference),
          reuseMaps_(reuseMaps) {}

    /**
     * Getter for desired no of threads
     * @return desired no of threads
     */
    [[nodiscard]] inline unsigned getThreads() const { return threads_; }
    /**
     * Getter for threshold for overlap to be meaningful
     * @return threshold
     */
    [[nodiscard]] inline double getThreshold() const { return threshold_; }
    /**
     * Getter for minimum map overlap length
     * @return minimum map overlap length
     */
    [[nodiscard]] inline unsigned getMinOverlap() const { return minOverlap_; }
    /**
     * Getter for minimum map overlap part
     * @return minimum map overlap part
     */
    [[nodiscard]] inline double getMinPart() const { return minPart_; }
    /**
     * Getter for minimum map length for map filtering
     * @return minimum map length
     */
    [[nodiscard]] inline size_t getMinMapLen() const {
      return 1 + static_cast<size_t>(minOverlap_ / minPart_);
    }
    /**
     * Getter for no of repeats
     * @return no of repeats
     */
    [[nodiscard]] inline unsigned getRepeats() const { return repeats_; }

    /**
     * Getter for marker preference
     * @return marker preference flag
     */
    [[nodiscard]] bool isMarkerPreference() const { return markerPreference_; }

    /**
     * Getter for reusage of maps
     * @return marker reusage of maps flag
     */
    [[nodiscard]] bool isReuseMaps() const { return reuseMaps_; }

  private:
    const unsigned threads_;
    const unsigned minOverlap_;
    const double minPart_;
    const double threshold_;
    const unsigned repeats_;
    const bool markerPreference_;
    const bool reuseMaps_;
  };
} // namespace bgm
#endif // BGM_ASM_ASSEMBLYPARAMS_H

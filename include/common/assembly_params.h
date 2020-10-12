//
// Created by przestaw on 12.10.2020.
//

#ifndef BGM_ASM_ASSEMBLYPARAMS_H
#define BGM_ASM_ASSEMBLYPARAMS_H

#include <string>
namespace oma {
  /**
   * Params for assembly task
   */
  class AssemblyParams {
  public:
    /**
     * Simplified default params constructor
     * @param inputFile input file
     * @param outputFile output file
     */
    AssemblyParams(std::string inputFile, std::string outputFile)
        : AssemblyParams(std::move(inputFile),
                         std::move(outputFile),
                         1,
                         200,
                         0.5,
                         0.1) {}

    /**
     * Constructor
     * @param inputFile input file
     * @param outputFile output file
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
    AssemblyParams(std::string inputFile,
                   std::string outputFile,
                   unsigned int threads,
                   unsigned int minOverlap,
                   double minPart,
                   double threshold,
                   unsigned repeats      = 0,
                   bool markerPreference = true,
                   bool reuseMaps        = false)
        : inputFile_(std::move(inputFile)), outputFile_(std::move(outputFile)),
          threads_(threads), minOverlap_(minOverlap), minPart_(minPart),
          threshold_(threshold), repeats_(repeats),
          markerPreference_(markerPreference), reuseMaps_(reuseMaps) {}

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
     * Getter for input filename
     * @return input filename
     */
    [[nodiscard]] inline const std::string& getInputFile() const {
      return inputFile_;
    }
    /**
     * Getter for output filename
     * @return output filename
     */
    [[nodiscard]] inline const std::string& getOutputFile() const {
      return outputFile_;
    }
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
    std::string inputFile_;
    std::string outputFile_;
    unsigned threads_;
    unsigned minOverlap_;
    double minPart_;
    double threshold_;
    unsigned repeats_;
    bool markerPreference_;
    bool reuseMaps_;
  };
} // namespace oma
#endif // BGM_ASM_ASSEMBLYPARAMS_H

//
// Created by przemek on 7/16/20.
//

#ifndef OMA_ALIGN_ALGORITHM_H
#define OMA_ALIGN_ALGORITHM_H

#include <data/maps_container.h>

namespace oma {
  class AlignAlgorithm {
  public:
    /* NOTE :
     *  Alignment == <shift, number of 1's after XOR>
     *  shift of 0 indicates no alignment
     *   -> [as shift 0 would mean duplicate this should not affect results]
     */
    using Alignment = std::pair<int64_t, size_t>;

    /**
     * Constructor
     * @param maps maps container reference
     * @param minOverlapQuants min no of Quants for overlap to be meaningful
     * @param minOverlapPart min part of map for overlap to be meaningful
     * @param threshold max no of diffrences for overlap to be meaningful
     */
    explicit AlignAlgorithm(MapsContainer &maps,
                            unsigned minOverlapQuants = 100,
                            double minOverlapPart     = 0.5,
                            double threshold          = 0.1);
    /**
     * Align maps using single thread
     */
    void alignMapsSingle();

    /**
     * Align maps using multiple threads
     * @param threads no of threads.
     * If 1, function will use single version.
     * If 0, function will detect available threads.
     */
    void alignMapsMulti(unsigned threads);

    /**
     * Aligns two maps
     * @param ref reference map
     * @param tested shifted map
     * @return offset between right maps ends, 0 means no meaningful alignment
     */
    [[nodiscard]] Alignment align(const BinaryMap::Bitset &ref,
                                  const BinaryMap::Bitset &tested) const;
    /*
     *
     * NOTE :     1.            2.                3.
     *  ref    :  [_________X],    [________XX],     [_______XXX]
     *  tested : [Y_________] , [YY________]   , [YYY_______]
     *            1.            2.                3.
     *  ref    :  [_________X],    [________XX],     [_______XXX]
     *  tested :  [_________] ,    [________]   ,    [_______]
     *            1.            2.                3.
     *  ref    :    [_________X],    [________XX],       [_______XXX]
     *  tested : [YYY_________] ,   [Y________]  , [YYYYYY_______]
     */

  private:
    // helper function aligning one neighbour of one map
    void alignOneMap(size_t index);
    // helper function taking next map after finishing previous
    void workerThread();
    // helper function taking range of maps to align
    void rangeWorkerThread(size_t begin, size_t end);

    // maps
    MapsContainer &maps_;

    // params
    const unsigned minOverlapQuants_;
    const double minOverlapPart_;
    const double threshold_;

    // scoring function
    std::function<double(uint64_t, uint64_t, uint64_t, uint64_t)>
        scoringFunction_;
    // default scoring function
    [[nodiscard]] double defaultScore(uint64_t diff,
                                      uint64_t ovlpLen,
                                      uint64_t mapLen1,
                                      uint64_t mapLen2) const {
      // diff compared to overlap : diff Proportional
      const double diffP =
          static_cast<double>(diff) / static_cast<double>(ovlpLen);

      if (diffP >= threshold_) {
        return 0.;
      } else {
        // diff compared to whole (smaller) map len : diff All
        const double diffA = static_cast<double>(diff)
                             / static_cast<double>(std::min(mapLen1, mapLen2));
        // TODO
        return diffP + diffA;
      }
    }
  };
} // namespace oma

#endif // OMA_ALIGN_ALGORITHM_H

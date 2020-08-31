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

    explicit AlignAlgorithm(MapsContainer &maps, unsigned minOverlap = 300);
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
     *
     * @param ref reference map
     * @param tested shifted map
     * @return offset between right maps ends, 0 means no meaningful alignment
     */
    [[nodiscard]] Alignment align(BinaryMap::Bitset ref,
                                  BinaryMap::Bitset tested) const;
  private:
    // helper function aligning one neighbour of one map
    void alignOneMap(size_t index);
    // helper function taking next map after finishing previous
    void workerThread();
    // helper function taking range of maps to align
    void rangeWorkerThread(size_t begin, size_t end);

    MapsContainer &_maps;
    unsigned _minOverlap;
  };
} // namespace oma

#endif // OMA_ALIGN_ALGORITHM_H

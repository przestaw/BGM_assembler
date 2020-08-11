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
    using Alignment = std::pair<unsigned, size_t>;

    explicit AlignAlgorithm(unsigned minOverlap = 300);
    void align_maps_single(MapsContainer &maps);

    /* NOTE :     1.            2.                3.
     *  ref    :  [_________X],    [________XX],     [_______XXX]
     *  tested : [Y_________] , [YY________]   , [YYY_______]
     *            1.            2.                3.
     *  ref    :  [_________X],    [________XX],     [_______XXX]
     *  tested :  [_________] ,    [________]   ,    [_______]
     *            1.            2.                3.
     *  ref    :    [_________X],    [________XX],       [_______XXX]
     *  tested : [YYY_________] ,   [Y________]  , [YYYYYY_______]
     *
     * NOTE : return shift of value 0 means no meaningful alignment
     */
    [[nodiscard]] Alignment align(BinaryMap::Bitset ref,
                                  BinaryMap::Bitset tested) const;

  private:
    void align_one_map(MapsContainer &maps, size_t index);

    unsigned minOverlap;
  };
} // namespace oma

#endif // OMA_ALIGN_ALGORITHM_H

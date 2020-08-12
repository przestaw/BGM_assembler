//
// Created by przemek on 7/16/20.
//

#include <algorithms/align_algorithm.h>
#include <iostream>

using namespace oma;

AlignAlgorithm::AlignAlgorithm(MapsContainer& maps, unsigned int minOverlap)
    : _maps(maps), _minOverlap(minOverlap) {}

void AlignAlgorithm::alignMapsSingle() {
  for (size_t iter = 0; iter < _maps.size(); ++iter) { alignOneMap(iter); }
}

void AlignAlgorithm::alignOneMap(size_t index) {
  // Right neighbour [if present - found as best right neighbour]
  if (_maps[index].getRightNeighbour().first < 0) {
    Alignment best   = Alignment(0, std::numeric_limits<size_t>::max());
    size_t neighbour = 0;
    /* NOTE : iterate from index to max
     *      - if pair with lower position it would be found earlier
     */
    for (size_t iter = 1 + index; iter < _maps.size(); ++iter) {
      Alignment result = align(_maps[index].getMap(), _maps[iter].getMap());
      // NOTE : if result of alignment is better save it as new best
      if (result.second < best.second) {
        best      = result;
        neighbour = iter;
      }
    }
    /* NOTE :
     *  value different than zero indicates significant alignment
     *  - see note in align function
     */
    if (best.first != 0) {
      _maps.addRightNeighbour(index, neighbour, best.first);
    }
  }
  // Left neighbour
  if (_maps[index].getLeftNeighbour().first < 0) {
    Alignment best   = Alignment(0, std::numeric_limits<size_t>::max());
    size_t neighbour = 0;
    /* NOTE : iterate from index to max
     *      - if pair with lower position it would be found earlier
     */
    for (size_t iter = 1 + index; iter < _maps.size(); ++iter) {
      // reuse align fun by switching arguments
      Alignment result = align(_maps[iter].getMap(), _maps[index].getMap());
      // NOTE : if result of alignment is better save it as new best
      if (result.second < best.second) {
        best      = result;
        neighbour = iter;
      }
    }
    /* NOTE :
     *  value different than zero indicates significant alignment
     *  - see note in align function
     */
    if (best.first != 0) {
      _maps.addLeftNeighbour(index, neighbour, best.first);
    }
  }
}

AlignAlgorithm::Alignment
AlignAlgorithm::align(BinaryMap::Bitset ref, BinaryMap::Bitset tested) const {
  // TODO : check or assure that maps are longer than minOverlap
  //        i.e. filter too short maps
  const size_t max_shift =
      1 + std::max(ref.size(), tested.size()) - _minOverlap;
  // init best as worst numerically possible
  size_t best       = std::numeric_limits<size_t>::max();
  size_t best_shift = 0;

  for (size_t shift = 1; shift < max_shift; ++shift) {
    BinaryMap::Bitset shifted = tested << shift;
    // make sure maps are equal length
    if (shifted.size() > ref.size()) {
      shifted.resize(ref.size());
    } else {
      ref.resize(shifted.size());
    }
    // NOTE :  XOR on bitset is only destructive
    shifted ^= ref;
    /* NOTE : assuming variance as in binominal distribution
     *  var = n * p * (1 - p)
     *  n = no of bits
     *  p = probability of 1 = 1/2
     *  var = n * 1/4
     *  3 sigma rule
     *  var * 3 = 3 * n / 4
     *  E(x) = n * p = n/2
     *  treshold = n/2 - 3n/4 = - n/4
     *
     * For p = 1/5
     * threshold = -0.32*n
     */
    size_t xor_result = shifted.count();
    // NOTE : must be calculated for each shift as n changes
    // size_t var_3 = 3 * shifted.size() / 4;
    // FIXME : temporary - 10% 1's as threshold
    if (xor_result < shifted.size() / 10) {
      // TODO : SCORE ???
      if (best > xor_result) {
        best       = xor_result;
        best_shift = shift;
      }
    }
  }

  return std::make_pair(best_shift, best);
}

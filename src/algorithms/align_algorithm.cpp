//
// Created by przemek on 7/16/20.
//

#include <algorithms/align_algorithm.h>
#include <functional>
#include <thread>

using namespace oma;

AlignAlgorithm::AlignAlgorithm(MapsContainer& maps,
                               unsigned int minOverlapQuants,
                               double minOverlapPart, double threshold)
    : _maps(maps), _minOverlapQuants(minOverlapQuants),
      _minOverlapPart(minOverlapPart), _threshold(threshold) {}

void AlignAlgorithm::alignMapsSingle() {
  // for (size_t iter = 0; iter < _maps.size(); ++iter) { alignOneMap(iter); }
  rangeWorkerThread(0, _maps.size());
}

void AlignAlgorithm::alignMapsMulti(unsigned int threadsCount) {
  const auto processorCount = std::thread::hardware_concurrency();

  if (threadsCount == 0) {
    threadsCount = processorCount;
  }

  if (threadsCount == 1) {
    // single thread
    alignMapsSingle();
  } else {
    // multi thread
    std::vector<std::thread> threads;

    // allocate memory for threads
    threads.reserve(threadsCount);
    // create threads
    for (unsigned i = 0; i < threadsCount; ++i) {
      threads.emplace_back(&AlignAlgorithm::workerThread, this);
    }

    // join threads
    for (auto& workerThread : threads) { workerThread.join(); }
  }
}

void AlignAlgorithm::alignOneMap(size_t index) {
  /* NOTE : finding all right neighbours comparing with all other maps for
   *        easier multithreading
   * NOTE : left neighbours are found in this process as relation
   *        is symmetrical
   */
  Alignment best   = Alignment(-1, std::numeric_limits<size_t>::max());
  size_t neighbour = 0;
  /* NOTE : iterate from index to max
   *      - if pair with lower position it would be found earlier
   */
  for (size_t iter = 0; iter < _maps.size(); ++iter) {
    if (iter != index) {
      // reuse align fun by switching arguments
      Alignment result = align(_maps[iter].getMap(), _maps[index].getMap());
      // NOTE : if result of alignment is better save it as new best
      if (result.second < best.second) {
        best      = result;
        neighbour = iter;
      }
    }
  }
  /* NOTE : value different than zero indicates significant alignment
   *        - see notes in align function
   */
  if (best.first >= 0) {
    _maps.addRightNeighbour(index, neighbour, best.first);
  }

  //  // Left neighbour
  //  if (_maps[index].getLeftNeighbour().first < 0) {
  //    Alignment best   = Alignment(-1, std::numeric_limits<size_t>::max());
  //    size_t neighbour = 0;
  //    /* NOTE : iterate from index to max
  //     *      - if pair with lower position it would be found earlier
  //     */
  //    for (size_t iter = index + 1; iter < _maps.size(); ++iter) {
  //      Alignment result = align(_maps[index].getMap(), _maps[iter].getMap());
  //      // NOTE : if result of alignment is better save it as new best
  //      if (result.second < best.second) {
  //        best      = result;
  //        neighbour = iter;
  //      }
  //    }
  //    /* NOTE : value different than zero indicates significant alignment
  //     *        - see notes in align function
  //     */
  //    if (best.first >= 0) {
  //      _maps.addLeftNeighbour(index, neighbour, best.first);
  //    }
  //  }
  //  // Right neighbour [if present - found as best left neighbour]
  //  if (_maps[index].getRightNeighbour().first < 0) {
  //    Alignment best   = Alignment(-1, std::numeric_limits<size_t>::max());
  //    size_t neighbour = 0;
  //    /* NOTE : iterate from index to max
  //     *      - if pair with lower position it would be found earlier
  //     */
  //    for (size_t iter = index + 1; iter < _maps.size(); ++iter) {
  //      // reuse align fun by switching arguments
  //      Alignment result = align(_maps[iter].getMap(), _maps[index].getMap());
  //      // NOTE : if result of alignment is better save it as new best
  //      if (result.second < best.second) {
  //        best      = result;
  //        neighbour = iter;
  //      }
  //    }
  //    /* NOTE : value different than zero indicates significant alignment
  //     *        - see notes in align function
  //     */
  //    if (best.first >= 0) {
  //      _maps.addRightNeighbour(index, neighbour, best.first);
  //    }
  //  }
}

AlignAlgorithm::Alignment
AlignAlgorithm::align(BinaryMap::Bitset ref,
                      const BinaryMap::Bitset& tested) const {
  // TODO : check or assure that maps are longer than minOverlap
  //        i.e. filter too short maps

  // FIXME : filter maps by len
  if (std::min(ref.size(), tested.size()) <= _minOverlapQuants) {
    return std::make_pair(-1, std::numeric_limits<size_t>::max());
  }

  // calc max shift
  const size_t smallerLen = std::min(ref.size(), tested.size());
  const size_t max_shift =
      std::min(1 + smallerLen - _minOverlapQuants,
               static_cast<size_t>(smallerLen * (1 - _minOverlapPart)));

  // init best as worst numerically possible
  size_t best        = std::numeric_limits<size_t>::max();
  int64_t best_shift = -1;
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
  // FIXME : temporary - 10% 1's as threshold

  auto performXorAndUpdate = [&](oma::BinaryMap::Bitset shiftedTest,
                                 const size_t shift) {
    // resize to equal lengths
    if (shiftedTest.size() < ref.size()) {
      ref.resize(shiftedTest.size());
    }

    shiftedTest ^= ref;
    const size_t xor_result = shiftedTest.count();
    const size_t margin     = shiftedTest.size() * _threshold;

    if (xor_result < margin) {
      // TODO : SCORE ???
      if (best > xor_result) {
        best       = xor_result;
        best_shift = shift;
      }
    }
  };

  // different approach for longer ref and longer testmap
  if (ref.size() <= tested.size()) {
    std::size_t diff = tested.size() - ref.size();
    for (size_t shift = 0; shift < max_shift; ++shift) {
      // NOTE : bitset has different direction -> lowest & highest position
      // NOTE : use diff to move R ends together
      oma::BinaryMap::Bitset shiftedTest = tested >> (shift + diff);
      // resize to correct value
      shiftedTest.resize(tested.size() - (shift + diff));

      /* resize to equal lengths
       * perform xor
       * count diffrences and compare with margin & best
       */
      performXorAndUpdate(shiftedTest, shift);
    }
  } else { // reference is longer
    std::size_t diff = ref.size() - tested.size();
    // NOTE : bitset has different direction -> lowest & highest position
    // NOTE : use diff to move R ends together
    ref = ref >> diff;

    for (size_t shift = 0; shift < max_shift; ++shift) {
      // NOTE : bitset has different direction -> lowest & highest position
      oma::BinaryMap::Bitset shiftedTest = tested >> shift;
      // resize to correct value
      shiftedTest.resize(tested.size() - shift);

      /* resize to equal lengths
       * perform xor
       * count diffrences and compare with margin & best
       */
      performXorAndUpdate(shiftedTest, shift);
    }
  }

  return std::make_pair(best_shift, best);
}

void AlignAlgorithm::workerThread() {
  size_t index = _maps.getUnaligned();
  while (index < _maps.size()) {
    alignOneMap(index);
    index = _maps.getUnaligned();
  }
}

void AlignAlgorithm::rangeWorkerThread(size_t begin, size_t end) {
  for (size_t iter = begin; iter < end; ++iter) { alignOneMap(iter); }
}

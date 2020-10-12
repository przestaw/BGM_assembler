//
// Created by przemek on 7/16/20.
//

#include <algorithms/align_algorithm.h>
#include <functional>
#include <thread>

using namespace oma;

AlignAlgorithm::AlignAlgorithm(MapsContainer& maps,
                               unsigned int minOverlapQuants,
                               double minOverlapPart,
                               double threshold)
    : maps_(maps), minOverlapQuants_(minOverlapQuants),
      minOverlapPart_(minOverlapPart), threshold_(threshold),
      scoringFunction_([&](uint64_t diff,
                           uint64_t ovlpLen,
                           uint64_t mapLen1,
                           uint64_t mapLen2) {
        return defaultScore(diff, ovlpLen, mapLen1, mapLen2);
      }) {}
//      scoringFunction(std::bind(&AlignAlgorithm::defaultScore, this,
//                                std::placeholders::_1, std::placeholders::_2,
//                                std::placeholders::_3, std::placeholders::_4))
//                                {}

void AlignAlgorithm::alignMapsSingle() { rangeWorkerThread(0, maps_.size()); }

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
  for (size_t iter = 0; iter < maps_.size(); ++iter) {
    if (iter != index) {
      // reuse align fun by switching arguments
      Alignment result = align(maps_[iter].getMap(), maps_[index].getMap());
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
    maps_.addRightNeighbour(index, neighbour, best.first);
  }
}

AlignAlgorithm::Alignment
AlignAlgorithm::align(const BinaryMap::Bitset& ref_,
                      const BinaryMap::Bitset& tested) const {
  // TODO  : check or assure that maps are longer than minOverlap
  //         i.e. filter too short maps
  // FIXME : filter maps by len

  // copy as it will be resized
  BinaryMap::Bitset ref(ref_);

  if (std::min(ref.size(), tested.size()) <= minOverlapQuants_) {
    return std::make_pair(-1, std::numeric_limits<size_t>::max());
  }

  // calc max shift
  const size_t refLen     = ref.size();
  const size_t testedLen  = tested.size();
  const size_t smallerLen = std::min(refLen, testedLen);
  const size_t max_shift =
      std::min(1 + smallerLen - minOverlapQuants_,
               static_cast<size_t>(smallerLen * (1 - minOverlapPart_)));

  // init best as worst numerically possible
  size_t best         = std::numeric_limits<size_t>::max();
  double best_quality = this->threshold_;
  int64_t best_shift  = -1;

  auto performXorAndUpdate = [&](oma::BinaryMap::Bitset shiftedTest,
                                 const size_t shift) {
    // resize to equal lengths
    if (shiftedTest.size() < ref.size()) {
      ref.resize(shiftedTest.size());
    }

    shiftedTest ^= ref;
    const size_t xor_result = shiftedTest.count();
    const double q = static_cast<double>(xor_result) / shiftedTest.size();

    // this->scoringFunction_(shiftedTest.count(), shiftedTest.size(), refLen,
    // testedLen);

    // above threshold or better 'quality' alignment
    if (q < best_quality) {
      // TODO : SCORE ???
      if (best > xor_result) {
        best         = xor_result;
        best_shift   = shift;
        best_quality = q;
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

      /* Note :
       *     - resize to equal lengths
       *     - perform xor
       *     - count diffrences and compare with margin & best
       */
      performXorAndUpdate(shiftedTest, shift);
    }
  }

  return AlignAlgorithm::Alignment(best_shift, best);
}

void AlignAlgorithm::workerThread() {
  size_t index = maps_.getUnaligned();
  while (index < maps_.size()) {
    alignOneMap(index);
    index = maps_.getUnaligned();
  }
}

void AlignAlgorithm::rangeWorkerThread(size_t begin, size_t end) {
  for (size_t iter = begin; iter < end; ++iter) { alignOneMap(iter); }
}

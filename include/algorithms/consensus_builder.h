//
// Created by przemek on 8/7/20.
//

#ifndef OMA_CONSENSUS_BUILDER_H
#define OMA_CONSENSUS_BUILDER_H

#include <data/maps_container.h>
#include <deque>
#include <list>
#include <mutex>
#include <vector>

namespace bgm {
  class ConsensusBuilder {
  public:
    /**
     * Constructor
     * @param maps maps container reference
     * @param markerPreference prefer marker or blank place
     * @param processAll process all sequences or only containing more than 1
     * map
     */
    explicit ConsensusBuilder(MapsContainer &maps,
                              bool markerPreference = true,
                              bool processAll       = true);

    /**
     * Runs consensus in single thread
     */
    void runConsensusSingle();

    /**
     * Runs consensus in desired no of threads
     * @param threads desired no of threads,
     *                0 indicates all available threads on machine,
     *                other values will create desired no of threads
     */
    void runMultiConsensus(unsigned threads);

    /**
     * Getter for resulting consensus maps
     * @return resulting consensus maps
     */
    [[nodiscard]] const std::vector<BinaryMap> &getConsensusMaps() const;

    /**
     * Reset and clear object
     */
    void reset();

    [[nodiscard]] std::vector<BinaryMap> getConsensusMapsAndReset();

  private:
    // worker thread
    void workerThread();

    // contig manipulation
    void detectContigs();
    void breakCycles();
    void breakOneCycle(int map);

    // consensus
    void makeConsensus(int lastMap); // will produce 'inverted' contigs
    bool votePosition(std::list<std::pair<std::size_t, int>> &voters);

    // contigs
    std::mutex congigLock_;
    std::deque<size_t> contigs_;

    // consensus maps;
    std::vector<BinaryMap> consensusMaps_;

    // output
    std::mutex consensusLock;
    void addReadyMap(const BinaryMap::Bitset &readyMap);

    // input
    MapsContainer &conMaps_;

    // marker deduction
    bool markerPreferenceFlag_;

    // lambda for marker deduction
    std::function<bool(size_t, size_t)> deductMarker_;

    // lambdas for marker deduction
    constexpr static const auto markerPreference_ = [](size_t count, size_t ones) {
      return (count - ones) <= ones;
    };
    constexpr static const auto noMarkerPreference_ = [](size_t count, size_t ones) {
      return (count - ones) < ones;
    };

    // process all or contigs
    bool processAll_;

    // lambdas for contig map count deduction
    const std::function<size_t(size_t)> countLenL = [&](size_t mapId) {
      size_t len;
      for (len   = 1; conMaps_[mapId].getLeftNeighbour().first >= 0;
           mapId = conMaps_[mapId].getLeftNeighbour().first) {
        ++len;
      }
      return len;
    };
  };
} // namespace bgm

#endif // OMA_CONSENSUS_BUILDER_H

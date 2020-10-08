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

namespace oma {
  class ConsensusBuilder {
  public:
    /**
     * Constructor
     * @param maps maps container reference
     */
    explicit ConsensusBuilder(MapsContainer &maps);

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
  };
} // namespace oma

#endif // OMA_CONSENSUS_BUILDER_H

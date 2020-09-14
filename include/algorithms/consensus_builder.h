//
// Created by przemek on 8/7/20.
//

#ifndef OMA_CONSENSUS_BUILDER_H
#define OMA_CONSENSUS_BUILDER_H

#include <data/maps_container.h>
#include <mutex>
#include <deque>
#include <list>
#include <vector>

namespace oma {
  class ConsensusBuilder {
  public:
    explicit ConsensusBuilder(MapsContainer &maps);

    void runConsensusSingle();
    void runMultiConsensus(unsigned threads);

    [[nodiscard]] const std::vector<BinaryMap> &getConsensusMaps() const;

  private:
    void workerThread();

    /*
     * detect (and sort) Contigs of maps
     *
     * NOTE: maybe revers direction instead of sorting
     * NOTE: remeber about detection of cycles
     *       -> Maybe extra fields in BinaryMap or MapsContainer
     *       -> break cycle through least trustful connection
     *
     * TODO: container for produced "lists" with new "coordinates"
     * TODO: lenght, 0's/1's count in "neighbourhood"
     *
     * NOTE: use statistics of "neighbourhood" to determine least trustful
     */
    void detectContigs();
    void breakCycles();
    void breakOneCycle(int map);

    /*
     * Make consensus -> for each of contigs
     *
     * NOTE: intention to be thread-independent
     *       -> run on 1 thread per each contig on as many cores as available
     * NOTE: std::thread + std::future?
     * NOTE: std::async ?
     * NOTE: http://www.davidespataro.it/cpp-concurrency-threads-future/
     *
     * NOTE: could be made const by returning resulting BinaryMap
     * NOTE: synchronised appends?
     */
    // void makeConsensus(contig);
    void makeConsensus(int lastMap); // will produce 'inverted' contigs
    bool votePosition(std::list<std::pair<std::size_t, int>> &voters);
    /*
     * TODO: container and filed for 'contigs'
     *       -> metaobjects [refering maps in maps container]
     * NOTE: vector could allow for 'empty' state detection
     * NOTE: could function as LIFO queue for makeConsensus
     */
    // std::vector<Contig> contigs;
    std::mutex congigLock;
    std::deque<size_t> contigs;

    /*
     * TODO: container for resulting consensus maps?
     *       -> maybe just another MapsContainer
     * NOTE: synchronised appends?
     * NOTE: std::thread + std::future?
     * NOTE: std::async ?
     */
    // MapsContainer consensusMaps;
    std::vector<BinaryMap> consensusMaps;

    // output
    std::mutex consensusLock;
    void addReadyMap(const BinaryMap::Bitset &readyMap);

    // input
    MapsContainer &conMaps;
  };
} // namespace oma

#endif // OMA_CONSENSUS_BUILDER_H

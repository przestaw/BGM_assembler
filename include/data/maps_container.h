//
// Created by przemek on 7/15/20.
//

#ifndef OMA_MAPS_CONTAINER_H
#define OMA_MAPS_CONTAINER_H

#include <data/bin_map.h>
#include <vector>
#include <mutex>

namespace oma {
  class MapsContainer : public std::vector<BinaryMap> {
  public:
    explicit MapsContainer(
        std::vector<BinaryMap> maps = std::vector<BinaryMap>());
    /**
     * Set neighbourhood for 2 maps
     * @param map map
     * @param neighbour left map neighbour
     * @param shift shift between right ends
     */
    void addLeftNeighbour(size_t map, size_t neighbour, unsigned shift);
    /**
     * Set neighbourhood for 2 maps
     * @param map map
     * @param neighbour left map neighbour
     * @param shift shift between right ends
     */
    void addRightNeighbour(size_t map, size_t neighbour, unsigned shift);
    /**
     * Breaks link between two maps
     * @param left left map
     * @param right right map
     */
    void breakLink(size_t left, size_t right);
    /**
     * Get next unaligned map index
     * @return unaligned map index
     */
    size_t getUnaligned();

    /* TODO: - synchronized access to map neighbours
     *          ? -> in maps (rwlock) or through maps container
     *       - (?) different access than mc[index].method()
     *       - get list(s) : null <-> M1 <-> M2 <-> ... <-> null
     *          ? -> could be handled by 2 bitsets keeping track of empty
     *              "neighbours"
     *       - ???
     */
  private:
    // used to synchronize neighbour addition
    std::mutex additionGuard;
    // points next unaligned map
    size_t unalignedPivot;
    // used to synchronize unaligned pivot usage
    std::mutex unalignedGuard;
  };
} // namespace oma

#endif // OMA_MAPS_CONTAINER_H
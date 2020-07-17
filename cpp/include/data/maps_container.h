//
// Created by przemek on 7/15/20.
//

#ifndef OMA_MAPS_CONTAINER_H
#define OMA_MAPS_CONTAINER_H

#include <data/bin_map.h>
#include <vector>

namespace oma {
  class MapsContainer : public std::vector<BinaryMap> {
  public:
    explicit MapsContainer(std::vector<BinaryMap> maps);
    // NOTE : set neighbour for both
    void addLeftNeighbour(size_t map, size_t neighbour, unsigned shift);
    void addRightNeighbour(size_t map, size_t neighbour, unsigned shift);
    /* TODO: - synchronized access to map neighbours
     *          ? -> in maps (rwlock) or through maps container
     *       - (?) different access than mc[index].method()
     *       - get list(s) : null <-> M1 <-> M2 <-> ... <-> null
     *          ? -> could be handled by 2 bitsets keeping track of empty
     *              "neighbours"
     *       - ???
     */
  private:
    // Synchronization primitives ??
    // Bitsets for track of empty neighbours ??
  };
} // namespace oma

#endif // OMA_MAPS_CONTAINER_H
//
// Created by przemek on 7/15/20.
//

#ifndef OMA_MAPS_CONTAINER_H
#define OMA_MAPS_CONTAINER_H

#include <vector>
#include "bin_map.h"

namespace oma::data{
  class MapsContainer : public std::vector<BinaryMap>{
  public:
    explicit MapsContainer(std::vector<BinaryMap> maps);
    // NOTE : set neighbour for both
    void addLNeighbour(size_t map, size_t neighbour, unsigned shift);
    void addRNeighbour(size_t map, size_t neighbour, unsigned shift);
    /* TODO: - synchronized access to map neighbours
     *          ? -> in maps (rwlock) or through maps container
     *       - (?) different access than mc[index].method()
     *       - get list(s) : null <-> M1 <-> M2 <-> ... <-> null
     *       - ???
     */
  private:
    // Synchronization primitives ??
  };
}

#endif // OMA_MAPS_CONTAINER_H

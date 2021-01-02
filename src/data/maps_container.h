//
// Created by przemek on 7/15/20.
//

#ifndef OMA_MAPS_CONTAINER_H
#define OMA_MAPS_CONTAINER_H

#include <data/bin_map.h>
#include <mutex>
#include <vector>

using std::vector;

namespace bgm {
  /**
   * Wrapper class for BinaryMap vector with additional features
   */
  class MapsContainer : public vector<BinaryMap> {
  public:
    /**
     * Constructor
     * @param maps maps to be stored
     */
    explicit MapsContainer(std::vector<BinaryMap> maps = std::vector<BinaryMap>());
    /**
     * Set neighbourhood for 2 maps
     * @param map map
     * @param neighbour left map neighbour
     */
    void addRightNeighbour(size_t map, LayoutAlignment neighbour);
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

    /**
     * Resets container to allow next iteration. Sets unaligned pivot back to
     * the beginning.
     */
    void reset();

    /**
     * Resets container and replaces content
     * @param maps new maps
     */
    void resetAndReplace(const std::vector<BinaryMap> &maps);

  private:
    // used to synchronize neighbour addition
    mutable std::mutex additionGuard_;
    // points next unaligned map
    size_t unalignedPivot_;
    // used to synchronize unaligned pivot usage
    mutable std::mutex unalignedGuard_;
    // used to lock mutex only once
    void internalAddRight(size_t map, LayoutAlignment align);
  };
} // namespace bgm

#endif // OMA_MAPS_CONTAINER_H
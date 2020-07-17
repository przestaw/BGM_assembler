//
// Created by przemek on 15.07.2020.
//

#ifndef BIN_MAP_H
#define BIN_MAP_H

#include <boost/dynamic_bitset.hpp>
#include <cstdint>

using boost::dynamic_bitset;

namespace oma {
  class BinaryMap {
  public:
    using Bitset    = dynamic_bitset<uint64_t>;
    using Neighbour = std::pair<BinaryMap *, unsigned>;
    /**
     * Constructor
     * @param identifier identifier for BinaryMap
     * @param map raw binary map
     */
    BinaryMap(const std::string &identifier, Bitset map);

    [[nodiscard]] const std::string &getIdentifier() const;
    // FIXME : index instead of pointer ?
    void setLeftNeighbour(BinaryMap *best, unsigned shift);
    void setRightNeighbour(BinaryMap *best, unsigned shift);
    Bitset getMap(); // NOTE : copy as it will be modified [shifted]
    const Bitset &getMap() const;
    [[nodiscard]] const Neighbour &getLeftNeighbour() const;
    [[nodiscard]] const Neighbour &getRightNeighbour() const;

  private:
    std::string identifier;
    Bitset map;
    // Left best aligned map -> TODO maybe index/id instead of pointer
    Neighbour leftNeighbour;
    // Right best aligned map
    Neighbour rightNeighbour;
  };
} // namespace oma

#endif // BIN_MAP_H
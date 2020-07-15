//
// Created by przemek on 15.07.2020.
//

#ifndef BIN_MAP_H
#define BIN_MAP_H

#include <boost/dynamic_bitset.hpp>
#include <cstdint>

using boost::dynamic_bitset;

namespace oma::data {
  class BinaryMap {
  public:
    using Bitset = dynamic_bitset<uint64_t>;
    /**
     * Constructor
     * @param identifier identifier for BinaryMap
     * @param map raw binary map
     */
    BinaryMap(const std::string &identifier, Bitset map);
    void setLeftBest(BinaryMap *best, unsigned shift);
    void setRightBest(BinaryMap *best, unsigned shift);
    Bitset getMap(); // NOTE : copy as it will be modified [shifted]
  private:
    std::string identifier;
    Bitset map;
    // Left best aligned map -> TODO maybe in different container
    BinaryMap *lBest = nullptr;
    unsigned lShift  = 0;
    // Right best aligned map -> TODO maybe in different container
    BinaryMap *rBest = nullptr;
    unsigned rShift  = 0;
  };
} // namespace oma::data

#endif // BIN_MAP_H
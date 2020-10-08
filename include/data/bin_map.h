//
// Created by przemek on 15.07.2020.
//

#ifndef BIN_MAP_H
#define BIN_MAP_H

#include <boost/dynamic_bitset.hpp>
#include <cstdint>

namespace oma {
  /**
   * Wraper for binary map sequence
   */
  class BinaryMap {
  public:
    using Bitset    = boost::dynamic_bitset<uint64_t>;
    using Neighbour = std::pair<int, unsigned>; // NOTE : pair <index, shift>
    /**
     * Constructor
     * @param identifier identifier for BinaryMap
     * @param map raw binary map
     */
    BinaryMap(std::string identifier, Bitset map);

    /**
     * Getter for map identifier
     * @return map identifier
     */
    [[nodiscard]] const std::string &getIdentifier() const;
    /**
     * Sets left neighbour to desired value
     * @param best best left neighbor
     * @param shift shift between neighbour right ends
     */
    void setLeftNeighbour(int best, unsigned shift);
    /**
     * Sets right neighbour to desired value
     * @param best best left neighbor
     * @param shift shift between neighbour right ends
     */
    void setRightNeighbour(int best, unsigned shift);
    /**
     * Getter for Bitmap of BinaryMap
     * @return copy of Bitmap for further use
     */
    [[nodiscard]] Bitset getMap();
    /**
     * Getter for Bitmap of BinaryMap
     * @return reference of Bitmap for further use
     */
    [[nodiscard]] const Bitset &getMap() const;
    /**
     * Getter for left neighbour information
     * @return left neighbour information
     */
    [[nodiscard]] const Neighbour &getLeftNeighbour() const;
    /**
     * Getter for right neighbour information
     * @return right neighbour information
     */
    [[nodiscard]] const Neighbour &getRightNeighbour() const;
    /**
     * Converts BinaryMap to FASTA-style record
     * @return string with record
     */
    [[nodiscard]] std::string toFastaRecord() const;

    /**
     * Getter for flag for use in algorithms
     * @return true if marked
     */
    [[nodiscard]] bool isMark() const;
    /**
     * Setter for flag for use in algorithms
     * @param mark Flag for use in algorithms
     */
    void setMark(bool mark = true);

  private:
    // data
    std::string identifier_;
    Bitset map_;
    // Left best aligned map -> TODO maybe index/id instead of pointer
    Neighbour leftNeighbour_;
    // Right best aligned map
    Neighbour rightNeighbour_;
    // flag for cycle breaking
    bool mark_;
  };
} // namespace oma

#endif // BIN_MAP_H
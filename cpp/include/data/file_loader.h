//
// Created by przemek on 15.07.2020.
//

#ifndef OMA_FILE_LOADER_H
#define OMA_FILE_LOADER_H

#include "bin_map.h"
#include <string>
#include <vector>

namespace oma {
  class FileLoader {
  private:
    using SingleRecord = std::tuple<bool, std::string, BinaryMap::Bitset>;

    using SequencePair = std::pair<BinaryMap::Bitset, BinaryMap::Bitset>;
    using ParsedRecord = std::tuple<bool, std::string, SequencePair>;

  public:
    // FIXME : use 2 maps [with reverse map] function variants
    // FIXME : filter maps
    static std::vector<BinaryMap> loadFile(std::istream& input,
                                           size_t minLen = 1);

  private:
    // FIXME : remove single maps variants
    static inline SingleRecord parseBinMap(std::istream& in);
    static inline BinaryMap::Bitset getSequence(std::istream& in);

    static inline ParsedRecord parseBinMaps(std::istream& in);
    static inline SequencePair getSequences(std::istream& in);
  };
} // namespace oma

#endif // OMA_FILE_LOADER_H

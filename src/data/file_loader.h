//
// Created by przemek on 15.07.2020.
//

#ifndef OMA_FILE_LOADER_H
#define OMA_FILE_LOADER_H

#include <data/bin_map.h>
#include <string>
#include <vector>

namespace bgm {
  class FileLoader {
  private:
    using SingleRecord = std::optional<std::pair<std::string, BinaryMap::Bitset>>;

    using SequencePair = std::pair<BinaryMap::Bitset, BinaryMap::Bitset>;
    using ParsedRecord = std::optional<std::pair<std::string, SequencePair>>;

  public:
    // FIXME : use 2 maps [with reverse map] function variants
    // FIXME : filter maps
    static std::vector<BinaryMap> loadFile(std::istream& input, size_t minLen = 1);

  private:
    // FIXME : remove single maps variants
    static inline SingleRecord parseBinMap(std::istream& in);
    static inline BinaryMap::Bitset getSequence(std::istream& in);

    static inline ParsedRecord parseBinMaps(std::istream& in);
    static inline SequencePair getSequences(std::istream& in);
  };
} // namespace bgm

#endif // OMA_FILE_LOADER_H

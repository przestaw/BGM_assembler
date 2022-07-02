//
// Created by przemek on 15.07.2020.
//

#ifndef OMA_FILE_LOADER_H
#define OMA_FILE_LOADER_H

#include <data/bin_map.h>
#include <string>
#include <vector>

namespace bgm {
  /**
   * Loader for FASTA01 / binary map input streams
   */
  class FileLoader {
  private:
    using SequencePair = std::pair<BinaryMap::Bitset, BinaryMap::Bitset>;
    using ParsedRecord = std::optional<std::pair<std::string, SequencePair>>;

  public:
    /**
     * Loads binary maps (along with their reverse complements) from input stream
     * @param input input stream containing FASTA01 records
     * @param minLen minimum length (in quants) required to keep a map
     * @return vector of loaded BinaryMap objects
     */
    static std::vector<BinaryMap> loadFile(std::istream& input, size_t minLen = 1);

  private:
    static inline ParsedRecord parseBinMaps(std::istream& in);
    static inline SequencePair getSequences(std::istream& in);
  };
} // namespace bgm

#endif // OMA_FILE_LOADER_H

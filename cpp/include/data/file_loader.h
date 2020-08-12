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
    using ParsedRecord = std::tuple<bool, std::string, BinaryMap::Bitset>;

  public:
    static std::vector<BinaryMap> loadFile(std::istream& input);

  private:
    static inline ParsedRecord parseBinMap(std::istream& in);
    static inline BinaryMap::Bitset getSequence(std::istream& in);
  };
} // namespace oma::data

#endif // OMA_FILE_LOADER_H

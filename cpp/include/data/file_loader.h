//
// Created by przemek on 15.07.2020.
//

#ifndef OMA_FILE_LOADER_H
#define OMA_FILE_LOADER_H

#include "bin_map.h"
#include <string>
#include <vector>

namespace oma::data {
  class FileLoader {
  public:
    // TODO : Container for BinaryMaps - vector method as private
    static std::vector<BinaryMap> loadFile(const std::string& filename);

  private:
    BinaryMap parseBinaryMap(std::istream& in);
  };
} // namespace oma::data

#endif // OMA_FILE_LOADER_H

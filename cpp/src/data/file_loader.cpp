//
// Created by przemek on 7/15/20.
//

#include <data/file_loader.h>
#include <fstream>

using namespace oma::data;
std::vector<BinaryMap> FileLoader::loadFile(const std::string& filename) {
  std::vector<BinaryMap> ret;

  return ret;
}
BinaryMap FileLoader::parseBinaryMap(std::istream& in) {
  std::string nameLine;
  getline(in, nameLine, '\n');

  // TODO : read lines of ~80 symbols and convert to binary sequence

  return BinaryMap(nameLine, BinaryMap::Bitset());
}

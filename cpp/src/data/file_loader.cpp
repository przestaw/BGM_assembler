//
// Created by przemek on 7/15/20.
//

#include <data/file_loader.h>
#include <fstream>

using namespace oma;

std::vector<BinaryMap> FileLoader::loadFile(std::istream& input) {
  std::vector<BinaryMap> ret;

  for (auto temp = parseBinMap(input); std::get<0>(temp);
       temp      = parseBinMap(input)) {
    ret.emplace_back(std::get<1>(temp), std::get<2>(temp));
  }

  return ret;
}

FileLoader::ParsedRecord FileLoader::parseBinMap(std::istream& in) {
  std::string nameLine;
  if (in.peek() == '>') {
    getline(in, nameLine, '\n');
    return ParsedRecord(true, nameLine, FileLoader::getSequence(in));
  } else {
    return ParsedRecord(false, "", BinaryMap::Bitset());
  }
}

BinaryMap::Bitset FileLoader::getSequence(std::istream& in) {
  std::string seq, tmpLine;

  while (in.peek() != '>' && !in.eof()) {
    getline(in, tmpLine, '\n');
    seq += tmpLine;
  }

  // std::string reverse = seq;
  std::reverse(seq.begin(), seq.end());

  return BinaryMap::Bitset(seq);
  // TODO : return pair<seq, rev_seq>
}

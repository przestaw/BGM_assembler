//
// Created by przemek on 7/15/20.
//

#include <data/file_loader.h>
#include <fstream>

using namespace bgm;

std::vector<BinaryMap> FileLoader::loadFile(std::istream& input, size_t minLen) {
  std::vector<BinaryMap> ret;

  // /* 2 maps variant */
  for (auto temp = parseBinMaps(input); temp.has_value(); temp = parseBinMaps(input)) {
    if (temp->second.first.size() >= minLen) {
      ret.emplace_back(temp->first, temp->second.first);
      ret.emplace_back("reversed " + temp->first, temp->second.second);
    }
  }

  return ret;
}

FileLoader::ParsedRecord FileLoader::parseBinMaps(std::istream& in) {
  std::string nameLine;
  if (in.peek() == '>') {
    getline(in, nameLine, '\n');
    return ParsedRecord(std::make_pair(nameLine, FileLoader::getSequences(in)));
  } else {
    return std::nullopt;
  }
}

FileLoader::SequencePair FileLoader::getSequences(std::istream& in) {
  std::string seq;
  std::string tmpLine;

  while (in.peek() != '>' && !in.eof()) {
    getline(in, tmpLine, '\n');
    seq += tmpLine;
  }

  std::string rev_seq = seq;
  std::reverse(seq.begin(), seq.end());

  return SequencePair(seq, rev_seq);
}

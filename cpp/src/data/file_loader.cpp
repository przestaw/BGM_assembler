//
// Created by przemek on 7/15/20.
//

#include <data/file_loader.h>
#include <fstream>

using namespace oma;

std::vector<BinaryMap> FileLoader::loadFile(std::istream& input,
                                            size_t minLen) {
  std::vector<BinaryMap> ret;

  (void)minLen; // TODO : use

  for (auto temp = parseBinMap(input); std::get<0>(temp);
       temp      = parseBinMap(input)) {
    ret.emplace_back(std::get<1>(temp), std::get<2>(temp));
  }

  // 2 maps variant
  //  for (auto temp = parseBinMaps(input); std::get<0>(temp);
  //       temp      = parseBinMaps(input)) {
  //    ret.emplace_back(std::get<1>(temp), std::get<2>(temp).first);
  //    ret.emplace_back("reversed " + std::get<1>(temp),
  //    std::get<2>(temp).second);
  //  }

  return ret;
}

FileLoader::SingleRecord FileLoader::parseBinMap(std::istream& in) {
  std::string nameLine;
  if (in.peek() == '>') {
    getline(in, nameLine, '\n');
    return SingleRecord(true, nameLine, FileLoader::getSequence(in));
  } else {
    return SingleRecord(false, "", BinaryMap::Bitset());
  }
}

BinaryMap::Bitset FileLoader::getSequence(std::istream& in) {
  std::string seq, tmpLine;

  while (in.peek() != '>' && !in.eof()) {
    getline(in, tmpLine, '\n');
    seq += tmpLine;
  }

  // std::string rev_seq = seq;
  std::reverse(seq.begin(), seq.end());

  return BinaryMap::Bitset(seq);
  // std::string rev_seq = seq;
}

FileLoader::ParsedRecord FileLoader::parseBinMaps(std::istream& in) {
  std::string nameLine;
  if (in.peek() == '>') {
    getline(in, nameLine, '\n');
    return ParsedRecord(true, nameLine, FileLoader::getSequences(in));
  } else {
    return ParsedRecord(false, "", SequencePair());
  }
}

FileLoader::SequencePair FileLoader::getSequences(std::istream& in) {
  std::string seq, tmpLine;

  while (in.peek() != '>' && !in.eof()) {
    getline(in, tmpLine, '\n');
    seq += tmpLine;
  }

  std::string rev_seq = seq;
  std::reverse(seq.begin(), seq.end());

  return SequencePair(seq, rev_seq);
}

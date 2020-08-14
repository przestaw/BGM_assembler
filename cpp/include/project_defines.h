#ifndef PROJECT_DEFINES_H
#define PROJECT_DEFINES_H

#include <boost/dynamic_bitset.hpp>
#include <iostream>
#include <vector>

template <typename T> inline void printVec(std::vector<T> vec) {
  for (auto &it : vec) { std::cout << it << ", "; }
  std::cout << std::endl;
}

inline void printBit(boost::dynamic_bitset<uint64_t> vec) {
  for (size_t i = 0; i < vec.size(); ++i) { std::cout << vec[i]; }
  std::cout << std::endl;
}

#endif // PROJECT_DEFINES_H

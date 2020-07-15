#ifndef PROJECT_DEFINES_H
#define PROJECT_DEFINES_H

#include <iostream>
#include <vector>

template <typename T> inline void printVec(std::vector<T> vec) {
  for (auto &it : vec) { std::cout << it << ", "; }
  std::cout << std::endl;
}

#endif // PROJECT_DEFINES_H

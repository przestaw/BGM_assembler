//
// Created by przestaw on 10.11.2020.
//
#include <multi_compare_bitmaps.h>
#include <utility>

bgm::py::MultiCompareBitmaps::MultiCompareBitmaps(bgm::BinaryMap  ref)
    : ref_(std::move(ref)) {}

void bgm::py::MultiCompareBitmaps::addMap(bgm::BinaryMap map) {
  // lock to do not interfere with execution
  std::unique_lock lock(mapsGuard);
  maps_.push_back(std::move(map));
}

std::vector<bgm::py::MultiCompareBitmaps::MarkerType> bgm::py::MultiCompareBitmaps::alignMapToRef() {
  // lock to prevent map addition
  std::unique_lock lock(mapsGuard);
  // TODO
  return std::vector<MarkerType>();
}

//
// Created by przestaw on 10.11.2020.
//
#include <algorithms/align_algorithm.h>
#include <error/exeception.h>
#include <multi_compare_bitmaps.h>
#include <utility>

bgm::py::MultiCompareBitmaps::MultiCompareBitmaps(bgm::BinaryMap ref)
    : ref_(std::move(ref)) {}

void bgm::py::MultiCompareBitmaps::addMap(bgm::BinaryMap map) {
  // lock to do not interfere with execution
  std::unique_lock lock(mapsGuard);
  maps_.push_back(std::move(map));
}

std::vector<bgm::py::MultiCompareBitmaps::MarkerType>
bgm::py::MultiCompareBitmaps::alignMapsToRef(bool truncate) {
  // lock to prevent map addition
  std::unique_lock lock(mapsGuard);

  this->alignMaps();

  if (truncate) {
    this->adjustAndTruncate();
  } else {
    this->adjustAndElongate();
  }

  return adjusted;
}

bgm::py::MultiCompareBitmaps::MapAlignDescriptor::MapAlignDescriptor(
    const bgm::BinaryMap& ref,
    int shift)
    : ref(ref), shift(shift) {}

void bgm::py::MultiCompareBitmaps::alignMaps() {
  const size_t refLen = ref_.getMap().size();

  for (const auto& map : maps_) {
    const auto result =
        bgm::AlignAlgorithm::pyAlignFromBegins(ref_.getMap(), map.getMap());

    MapAlignDescriptor pivot(map);

    pivot.shift = result.getShift();
    const int r = refLen - pivot.shift - map.getMap().size();

    maxFo = (static_cast<int>(maxFo) > -pivot.shift) ? maxFo : -pivot.shift;
    maxRo = (static_cast<int>(maxRo) > -r) ? maxRo : -r;

    alignResults.push_back(pivot);
  }
}

void bgm::py::MultiCompareBitmaps::adjustAndElongate() {
  totLen = ref_.getMap().size() + maxFo + maxRo;

  for (size_t i = 0; i < totLen; ++i) {
    this->adjusted.push_back(bgm::py::getRefValAtPos(i, maxFo, ref_));
  }
  this->names.emplace_back(ref_.getIdentifier());

  for (const auto& pivot : alignResults) {
    const int start = pivot.shift + maxFo;
    for (size_t i = 0; i < totLen; ++i) {
      this->adjusted.push_back(bgm::py::getMapValAtPos(i, start, pivot.ref));
    }
    this->names.emplace_back(pivot.ref.getIdentifier());
  }
}

void bgm::py::MultiCompareBitmaps::adjustAndTruncate() {
  totLen = ref_.getMap().size();

  for (size_t i = 0; i < ref_.getMap().size(); ++i) {
    this->adjusted.push_back(bgm::py::getRefValAtPos(i, 0, ref_));
  }
  this->names.emplace_back(ref_.getIdentifier());

  for (const auto& pivot : alignResults) {
    const int start = pivot.shift;
    for (size_t i = 0; i < ref_.getMap().size(); ++i) {
      this->adjusted.push_back(bgm::py::getMapValAtPos(i, start, pivot.ref));
    }
    this->names.emplace_back(pivot.ref.getIdentifier());
  }
}

boost::python::list bgm::py::MultiCompareBitmaps::getNames() {
  std::unique_lock lock(mapsGuard);
  // result must be returned as python list
  boost::python::list list;
  // reuse code from AssemblyTask
  for (const auto& map : names) { list.append(map); }
  return list;
}

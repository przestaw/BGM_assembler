//
// Created by przemek on 7/15/20.
//
#include <data/maps_container.h>
#include <iostream>

using namespace bgm;

MapsContainer::MapsContainer(std::vector<BinaryMap> maps)
    : std::vector<BinaryMap>(std::move(maps)), unalignedPivot_(0) {}

// void MapsContainer::addLeftNeighbour(size_t map, size_t neighbour,
//                                     unsigned shift) {
//  // lock the container
//  std::unique_lock guard(additionGuard);
//
//  if ((*this)[map].getLeftNeighbour().first < 0
//      && (*this)[neighbour].getRightNeighbour().first < 0) {
//    (*this)[map].setLeftNeighbour(neighbour, shift);
//    (*this)[neighbour].setRightNeighbour(map, shift);
//  }
//}

void MapsContainer::interandAddRight(size_t map,
                                     size_t neighbour,
                                     unsigned shift) {
  bool rFlag = (*this)[map].getRightNeighbour().first < 0;
  bool lFlag = (*this)[neighbour].getLeftNeighbour().first < 0;

  // avoid 0-shift cycles
  if (shift == 0) {
    // NOTE : Code below is used to avoid creation of 0's length cycles
    auto lNeighbour = (*this)[map].getLeftNeighbour();
    while (lNeighbour.second == 0 && lNeighbour.first >= 0) {
      if (lNeighbour.first == static_cast<int>(neighbour)) {
        return;
      }
      lNeighbour = (*this)[lNeighbour.first].getLeftNeighbour();
    }
  }

  if (rFlag && lFlag) {
    (*this)[map].setRightNeighbour(neighbour, shift);
    (*this)[neighbour].setLeftNeighbour(map, shift);
  } else if (!rFlag && lFlag) {
    auto rNeighbor = (*this)[map].getRightNeighbour();
    if (rNeighbor.second > shift) {
      auto diff = rNeighbor.second - shift;

      // M -> N -> R
      (*this)[map].setRightNeighbour(neighbour, shift);
      (*this)[neighbour].setLeftNeighbour(map, shift);
      (*this)[rNeighbor.first].setLeftNeighbour(-1, 0);

      interandAddRight(neighbour, rNeighbor.first, diff);
    } else {
      auto diff = shift - rNeighbor.second;

      // M -> R -> N
      interandAddRight(rNeighbor.first, neighbour, diff);
    }
  } else if (rFlag) { // rFlag && !lFlag
    auto lNeighbor = (*this)[neighbour].getLeftNeighbour();
    if (lNeighbor.second > shift) {
      auto diff = lNeighbor.second - shift;

      (*this)[map].setRightNeighbour(neighbour, shift);
      (*this)[neighbour].setLeftNeighbour(map, shift);
      (*this)[lNeighbor.first].setRightNeighbour(-1, 0);

      interandAddRight(lNeighbor.first, map, diff);
    } else {
      auto diff = shift - lNeighbor.second;
      interandAddRight(map, lNeighbor.first, diff);
    }
  } else { // !rFlag && !lFlag
    auto rNeighbor = (*this)[map].getRightNeighbour();
    auto lNeighbor = (*this)[neighbour].getLeftNeighbour();

    if (rNeighbor.first != lNeighbor.first) {
      std::cerr << "M -> A ... B -> N\n";
    } // else : M -> ? -> N [OK]
  }
}

void MapsContainer::addRightNeighbour(size_t map,
                                      size_t neighbour,
                                      unsigned shift) {
  // lock the container
  std::unique_lock guard(additionGuard_);

  /*
  if ((*this)[map].getRightNeighbour().first < 0
      && (*this)[neighbour].getLeftNeighbour().first < 0) {
    (*this)[map].setRightNeighbour(neighbour, shift);
    (*this)[neighbour].setLeftNeighbour(map, shift);
  }
   */

  this->interandAddRight(map, neighbour, shift);
}

void MapsContainer::breakLink(size_t left, size_t right) {
  // lock the container
  std::unique_lock guard(additionGuard_);
  (*this)[left].setRightNeighbour(-1, std::numeric_limits<unsigned>::max());
  (*this)[right].setLeftNeighbour(-1, std::numeric_limits<unsigned>::max());
}

size_t MapsContainer::getUnaligned() {
  // lock pivot
  std::unique_lock guard(unalignedGuard_);
  // check if reached end
  if (unalignedPivot_ < size()) {
    return unalignedPivot_++;
  } else {
    return size();
  }
}

void MapsContainer::reset() { unalignedPivot_ = 0; }

void MapsContainer::resetAndReplace(const std::vector<BinaryMap>& maps) {
  vector<BinaryMap>* myMap = this;
  *myMap                   = maps;
  unalignedPivot_          = 0;
}

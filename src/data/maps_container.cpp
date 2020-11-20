//
// Created by przemek on 7/15/20.
//
#include <data/maps_container.h>
#include <iostream>

using namespace bgm;

MapsContainer::MapsContainer(std::vector<BinaryMap> maps)
    : std::vector<BinaryMap>(std::move(maps)), unalignedPivot_(0) {}

void MapsContainer::internalAddRight(size_t map, size_t neighbour, unsigned shift) {
  bool rFlag = !(*this)[map].getRightNeighbour().isValid();
  bool lFlag = !(*this)[neighbour].getLeftNeighbour().isValid();

  // avoid 0-shift cycles
  if (shift == 0) {
    // NOTE : Code below is used to avoid creation of 0's length cycles
    auto lNeighbour = (*this)[map].getLeftNeighbour();
    while (lNeighbour.getShift() == 0 && lNeighbour.getMap() >= 0) {
      if (lNeighbour.getMap() == static_cast<int>(neighbour)) {
        return;
      }
      lNeighbour = (*this)[lNeighbour.getMap()].getLeftNeighbour();
    }
  }

  if (rFlag && lFlag) {
    (*this)[map].setRightNeighbour(BinaryMap::Neighbour(neighbour, shift));
    (*this)[neighbour].setLeftNeighbour(BinaryMap::Neighbour(map, shift));
  } else if (!rFlag && lFlag) {
    auto rNeighbor = (*this)[map].getRightNeighbour();
    if (rNeighbor.getShift() > shift) {
      auto diff = rNeighbor.getShift() - shift;

      // M -> N -> R
      (*this)[map].setRightNeighbour(BinaryMap::Neighbour(neighbour, shift));
      (*this)[neighbour].setLeftNeighbour(BinaryMap::Neighbour(map, shift));
      (*this)[rNeighbor.getMap()].setLeftNeighbour(BinaryMap::Neighbour::notAligned());

      internalAddRight(neighbour, rNeighbor.getMap(), diff);
    } else {
      auto diff = shift - rNeighbor.getShift();

      // M -> R -> N
      internalAddRight(rNeighbor.getMap(), neighbour, diff);
    }
  } else if (rFlag) { // rFlag && !lFlag
    auto lNeighbor = (*this)[neighbour].getLeftNeighbour();
    if (lNeighbor.getShift() > shift) {
      auto diff = lNeighbor.getShift() - shift;

      (*this)[map].setRightNeighbour(BinaryMap::Neighbour(neighbour, shift));
      (*this)[neighbour].setLeftNeighbour(BinaryMap::Neighbour(map, shift));
      (*this)[lNeighbor.getMap()].setRightNeighbour(BinaryMap::Neighbour::notAligned());

      internalAddRight(lNeighbor.getMap(), map, diff);
    } else {
      auto diff = shift - lNeighbor.getShift();
      internalAddRight(map, lNeighbor.getMap(), diff);
    }
  } else { // !rFlag && !lFlag
    auto rNeighbor = (*this)[map].getRightNeighbour();
    auto lNeighbor = (*this)[neighbour].getLeftNeighbour();

    if (rNeighbor.getMap() != lNeighbor.getMap()) {
      std::cerr << "M -> A ... B -> N\n";
    } // else : M -> ? -> N [OK]
  }
}

void MapsContainer::addRightNeighbour(size_t map, size_t neighbour, unsigned shift) {
  // lock the container
  std::unique_lock guard(additionGuard_);

  this->internalAddRight(map, neighbour, shift);
}

void MapsContainer::breakLink(size_t left, size_t right) {
  // lock the container
  std::unique_lock guard(additionGuard_);
  (*this)[left].setRightNeighbour(BinaryMap::Neighbour::notAligned());
  (*this)[right].setLeftNeighbour(BinaryMap::Neighbour::notAligned());
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

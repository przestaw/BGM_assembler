//
// Created by przemek on 7/15/20.
//
#include <data/maps_container.h>
#include <iostream>

using namespace bgm;

MapsContainer::MapsContainer(std::vector<BinaryMap> maps)
    : std::vector<BinaryMap>(std::move(maps)), unalignedPivot_(0) {}

void MapsContainer::internalAddRight(size_t map, LayoutAlignment align) {
  bool rFlag = !(*this)[map].getRightNeighbour().isValid();
  bool lFlag = !(*this)[align.getMap()].getLeftNeighbour().isValid();

  // avoid 0-shift cycles
  if (align.getShift() == 0) {
    // NOTE : Code below is used to avoid creation of 0's length cycles
    auto lNeighbour = (*this)[map].getLeftNeighbour();
    while (lNeighbour.getShift() == 0 && lNeighbour.getMap() >= 0) {
      if (lNeighbour.getMap() == static_cast<int>(align.getMap())) {
        return;
      }
      lNeighbour = (*this)[lNeighbour.getMap()].getLeftNeighbour();
    }
  }

  if (rFlag && lFlag) {
    (*this)[map].setRightNeighbour(align);
    (*this)[align.getMap()].setLeftNeighbour(align.mirrorAlignment(map));
  } else if (!rFlag && lFlag) {
    auto rNeighbor = (*this)[map].getRightNeighbour();
    if (rNeighbor.getShift() > align.getShift()) {
      const auto diff = rNeighbor.getShift() - align.getShift();
      const auto qual = (align.getQuality() + rNeighbor.getQuality()) / 2;

      // M -> N -> R
      (*this)[map].setRightNeighbour(align);
      (*this)[align.getMap()].setLeftNeighbour(align.mirrorAlignment(map));
      (*this)[rNeighbor.getMap()].setLeftNeighbour(BinaryMap::Neighbour::notAligned());

      internalAddRight(align.getMap(),
                       BinaryMap::Neighbour(rNeighbor.getMap(), diff, qual));
    } else {
      const auto diff = align.getShift() - rNeighbor.getShift();
      const auto qual = (align.getQuality() + rNeighbor.getQuality()) / 2;

      // M -> R -> N
      internalAddRight(rNeighbor.getMap(),
                       BinaryMap::Neighbour(align.getMap(), diff, qual));
    }
  } else if (rFlag) { // rFlag && !lFlag
    auto lNeighbor = (*this)[align.getMap()].getLeftNeighbour();
    if (lNeighbor.getShift() > align.getShift()) {
      const auto diff = lNeighbor.getShift() - align.getShift();
      const auto qual = (align.getQuality() + lNeighbor.getQuality()) / 2;

      (*this)[map].setRightNeighbour(align);
      (*this)[align.getMap()].setLeftNeighbour(align.mirrorAlignment(map));
      (*this)[lNeighbor.getMap()].setRightNeighbour(BinaryMap::Neighbour::notAligned());

      internalAddRight(lNeighbor.getMap(), BinaryMap::Neighbour(map, diff, qual));
    } else {
      const auto diff = align.getShift() - lNeighbor.getShift();
      const auto qual = (align.getQuality() + lNeighbor.getQuality()) / 2;
      internalAddRight(map, BinaryMap::Neighbour(lNeighbor.getMap(), diff, qual));
    }
  } else { // !rFlag && !lFlag
    auto rNeighbor = (*this)[map].getRightNeighbour();
    auto lNeighbor = (*this)[align.getMap()].getLeftNeighbour();

    if (rNeighbor.getMap() != lNeighbor.getMap()) {
      std::cerr << "M -> A ... B -> N\n";
    } // else : M -> ? -> N [OK]
  }
}

void MapsContainer::addRightNeighbour(size_t map, LayoutAlignment align) {
  // lock the container
  std::unique_lock guard(additionGuard_);

  this->internalAddRight(map, align);
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

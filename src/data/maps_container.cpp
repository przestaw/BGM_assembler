//
// Created by przemek on 7/15/20.
//
#include <data/maps_container.h>
#include <iostream>

using namespace oma;

MapsContainer::MapsContainer(std::vector<BinaryMap> maps)
    : std::vector<BinaryMap>(std::move(maps)), unalignedPivot(0) {}

//void MapsContainer::addLeftNeighbour(size_t map, size_t neighbour,
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

void MapsContainer::interandAddRight(size_t map, size_t neighbour, unsigned shift) {
  bool rFlag = (*this)[map].getRightNeighbour().first < 0;
  bool lFlag = (*this)[neighbour].getLeftNeighbour().first < 0;

  if(shift == 0){
    if ((*this)[map].getLeftNeighbour().first == static_cast<int>(neighbour)){
      return;
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
    } else if (rNeighbor.second < shift) {
      auto diff = shift - rNeighbor.second;

      // M -> R -> N
      interandAddRight(rNeighbor.first, neighbour, diff);
    } else { // if ==
      interandAddRight(rNeighbor.first, neighbour, 0);
    }
  } else if(rFlag) { // rFlag && !lFlag
    auto lNeighbor = (*this)[neighbour].getLeftNeighbour();
    if (lNeighbor.second > shift) {
      auto diff = lNeighbor.second - shift;

      (*this)[map].setRightNeighbour(neighbour, shift);
      (*this)[neighbour].setLeftNeighbour(map, shift);
      (*this)[lNeighbor.first].setRightNeighbour(-1, 0);

      interandAddRight(lNeighbor.first, map, diff);
    } else if (lNeighbor.second < shift) {
      auto diff = shift - lNeighbor.second;
      interandAddRight(map, lNeighbor.first, diff);
    } else{ // if ==
      interandAddRight(map, lNeighbor.first, 0);
    }
  } else { // !rFlag && !lFlag
    auto rNeighbor = (*this)[map].getRightNeighbour();
    auto lNeighbor = (*this)[neighbour].getLeftNeighbour();

    if( rNeighbor.first != lNeighbor.first){
      std::cout << "M -> A ... B -> N\n";
    } // else : M -> ? -> N [OK]
  }
}

void MapsContainer::addRightNeighbour(size_t map, size_t neighbour,
                                      unsigned shift) {
  // lock the container
  std::unique_lock guard(additionGuard);

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
  std::unique_lock guard(additionGuard);
  (*this)[left].setRightNeighbour(-1, std::numeric_limits<unsigned>::max());
  (*this)[right].setLeftNeighbour(-1, std::numeric_limits<unsigned>::max());
}

size_t MapsContainer::getUnaligned() {
  // lock pivot
  std::unique_lock guard(unalignedGuard);
  // check if reached end
  if (unalignedPivot < size()) {
    return unalignedPivot++;
  } else {
    return size();
  }
}

void MapsContainer::resetAndReplace(const std::vector<BinaryMap>& maps) {
  vector<BinaryMap>* myMap = this;
  *myMap                   = maps;
  unalignedPivot           = 0;
}

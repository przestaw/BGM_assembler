//
// Created by przemek on 7/15/20.
//
#include <data/maps_container.h>

using namespace oma;

MapsContainer::MapsContainer(std::vector<BinaryMap> maps)
    : std::vector<BinaryMap>(std::move(maps)) {}

void MapsContainer::addLeftNeighbour(size_t map, size_t neighbour,
                                     unsigned shift) {
  if ((*this)[map].getLeftNeighbour().first < 0
      && (*this)[neighbour].getRightNeighbour().first < 0) {
    (*this)[map].setLeftNeighbour(neighbour, shift);
    (*this)[neighbour].setRightNeighbour(map, shift);
  }

  //  if((*this)[map].getLeftNeighbour().second > shift){
  //    if((*this)[neighbour].getRightNeighbour().second > shift){
  //      // TODO : better relation resolve OR graph ???
  //      // check map link
  //      if((*this)[map].getLeftNeighbour().first >= 0){
  //        breakLink((*this)[map].getLeftNeighbour().first, map);
  //        // check neighbor link link
  //      } else if((*this)[neighbour].getRightNeighbour().first >= 0){
  //        breakLink(neighbour, (*this)[neighbour].getRightNeighbour().first);
  //      }
  //
  //      (*this)[map].setLeftNeighbour(neighbour, shift);
  //      (*this)[neighbour].setRightNeighbour(map, shift);
  //    } // else one of maps has better link
  //  } // else one of maps has better link
}

void MapsContainer::addRightNeighbour(size_t map, size_t neighbour,
                                      unsigned shift) {
  if ((*this)[map].getRightNeighbour().first < 0
      && (*this)[neighbour].getLeftNeighbour().first < 0) {
    (*this)[map].setRightNeighbour(neighbour, shift);
    (*this)[neighbour].setLeftNeighbour(map, shift);
  }
  //  if((*this)[map].getRightNeighbour().second > shift){
  //    if((*this)[neighbour].getLeftNeighbour().second > shift){
  //      // TODO : better relation resolve OR graph ???
  //      // check map link
  //      if((*this)[map].getRightNeighbour().first >= 0){
  //        breakLink(map, (*this)[map].getRightNeighbour().first);
  //        // check neighbor link link
  //      } else if((*this)[neighbour].getLeftNeighbour().first >= 0){
  //        breakLink((*this)[neighbour].getLeftNeighbour().first, neighbour);
  //      }
  //
  //      (*this)[map].setRightNeighbour(neighbour, shift);
  //      (*this)[neighbour].setLeftNeighbour(map, shift);
  //    } // else one of maps has better link
  //  } // else one of maps has better link
}

void MapsContainer::breakLink(size_t left, size_t right) {
  (*this)[left].setRightNeighbour(-1, std::numeric_limits<unsigned>::max());
  (*this)[right].setLeftNeighbour(-1, std::numeric_limits<unsigned>::max());
}

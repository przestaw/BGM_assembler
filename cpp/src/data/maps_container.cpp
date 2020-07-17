//
// Created by przemek on 7/15/20.
//
#include <data/maps_container.h>

using namespace oma;

MapsContainer::MapsContainer(std::vector<BinaryMap> maps)
    : std::vector<BinaryMap>(std::move(maps)) {}

void MapsContainer::addLeftNeighbour(size_t map, size_t neighbour,
                                     unsigned shift) {
  (*this)[map].setLeftNeighbour(&(*this)[neighbour], shift);
  (*this)[neighbour].setRightNeighbour(&(*this)[map], shift);
}

void MapsContainer::addRightNeighbour(size_t map, size_t neighbour,
                                      unsigned shift) {
  (*this)[map].setRightNeighbour(&(*this)[neighbour], shift);
  (*this)[neighbour].setLeftNeighbour(&(*this)[map], shift);
}

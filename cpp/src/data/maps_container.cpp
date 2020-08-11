//
// Created by przemek on 7/15/20.
//
#include <data/maps_container.h>

using namespace oma;

MapsContainer::MapsContainer(std::vector<BinaryMap> maps)
    : std::vector<BinaryMap>(std::move(maps)) {}

void MapsContainer::addLeftNeighbour(size_t map, size_t neighbour,
                                     unsigned shift) {
  (*this)[map].setLeftNeighbour(neighbour, shift);
  (*this)[neighbour].setRightNeighbour(map, shift);
}

void MapsContainer::addRightNeighbour(size_t map, size_t neighbour,
                                      unsigned shift) {
  (*this)[map].setRightNeighbour(neighbour, shift);
  (*this)[neighbour].setLeftNeighbour(map, shift);
}

void MapsContainer::breakLink(size_t left, size_t right) {
  (*this)[left].setRightNeighbour(-1, 0);
  (*this)[right].setLeftNeighbour(-1, 0);
}

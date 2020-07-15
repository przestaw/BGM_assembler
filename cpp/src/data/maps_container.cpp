//
// Created by przemek on 7/15/20.
//
#include <data/maps_container.h>

using namespace oma::data;

MapsContainer::MapsContainer(std::vector<BinaryMap> maps)
    : std::vector<BinaryMap>(std::move(maps)) {}

void MapsContainer::addLNeighbour(size_t map, size_t neighbour, unsigned shift) {
  (*this)[map].setLeftBest(&(*this)[neighbour], shift);
  (*this)[neighbour].setRightBest(&(*this)[map], shift);
}

void MapsContainer::addRNeighbour(size_t map, size_t neighbour, unsigned shift) {
  (*this)[map].setRightBest(&(*this)[neighbour], shift);
  (*this)[neighbour].setLeftBest(&(*this)[map], shift);
}

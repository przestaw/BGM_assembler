//
// Created by przestaw on 28.09.2020.
//
#include <boost/test/unit_test.hpp>
#include <data/maps_container.h>

// struct GlobalRef{
// private:
//  std::vector<oma::BinaryMap> ref_vec = std::vector<oma::BinaryMap>(7,
//  oma::BinaryMap("", oma::BinaryMap::Bitset()));
// public:
//  oma::MapsContainer ref_maps = oma::MapsContainer(ref_vec);
//
//  GlobalRef(){
//    ref_maps.addRightNeighbour(0, 1, 10);
//    ref_maps.addRightNeighbour(1, 2, 10);
//    ref_maps.addRightNeighbour(2, 3, 10);
//    ref_maps.addRightNeighbour(3, 4, 10);
//    ref_maps.addRightNeighbour(4, 5, 10);
//    ref_maps.addRightNeighbour(5, 6, 10);
//    ref_maps.addRightNeighbour(6, 0, 10);
//  }
//} globalRef;

BOOST_AUTO_TEST_SUITE(OMA_Test_Suite)

BOOST_AUTO_TEST_SUITE(Neighbour_Add_Suite)

BOOST_AUTO_TEST_CASE(MisedAlignTest_Circle_1) {
  std::vector<oma::BinaryMap> vec(7,
                                  oma::BinaryMap("", oma::BinaryMap::Bitset()));
  oma::MapsContainer maps(vec);

  maps.addRightNeighbour(0, 1, 10);
  maps.addRightNeighbour(1, 2, 10);

  maps.addRightNeighbour(2, 4, 20);

  maps.addRightNeighbour(4, 5, 10);
  maps.addRightNeighbour(5, 6, 10);
  maps.addRightNeighbour(6, 0, 10);

  // tested method should resolve 2 -(20)- 4 && 2 -(10)- 3
  // =>
  // 2 -(10)- 3 -(10)- 4
  maps.addRightNeighbour(2, 3, 10);

  BOOST_CHECK_EQUAL(maps[2].getRightNeighbour().first, 3);
  BOOST_CHECK_EQUAL(maps[2].getRightNeighbour().second, 10);

  BOOST_CHECK_EQUAL(maps[3].getRightNeighbour().first, 4);
  BOOST_CHECK_EQUAL(maps[3].getRightNeighbour().second, 10);
}

BOOST_AUTO_TEST_CASE(MisedAlignTest_Circle_2) {
  std::vector<oma::BinaryMap> vec(7,
                                  oma::BinaryMap("", oma::BinaryMap::Bitset()));
  oma::MapsContainer maps(vec);

  maps.addRightNeighbour(0, 1, 10);
  maps.addRightNeighbour(1, 2, 10);

  maps.addRightNeighbour(2, 4, 20);

  maps.addRightNeighbour(4, 5, 10);
  maps.addRightNeighbour(5, 6, 10);
  maps.addRightNeighbour(6, 0, 10);

  // tested method should resolve 2 -(20)- 4 && 3 -(10)- 4
  // =>
  // 2 -(10)- 3 -(10)- 4
  maps.addRightNeighbour(3, 4, 10);

  BOOST_CHECK_EQUAL(maps[2].getRightNeighbour().first, 3);
  BOOST_CHECK_EQUAL(maps[2].getRightNeighbour().second, 10);

  BOOST_CHECK_EQUAL(maps[3].getRightNeighbour().first, 4);
  BOOST_CHECK_EQUAL(maps[3].getRightNeighbour().second, 10);
}

BOOST_AUTO_TEST_CASE(AnotherReferedAlignTest_Circle) {
  std::vector<oma::BinaryMap> vec(7,
                                  oma::BinaryMap("", oma::BinaryMap::Bitset()));
  oma::MapsContainer maps(vec);

  maps.addRightNeighbour(0, 1, 10);
  maps.addRightNeighbour(1, 2, 10);
  maps.addRightNeighbour(2, 3, 10);
  maps.addRightNeighbour(3, 4, 10);
  maps.addRightNeighbour(4, 5, 10);
  maps.addRightNeighbour(5, 6, 10);
  maps.addRightNeighbour(6, 0, 10);

  maps.addRightNeighbour(2, 4, 20);

  BOOST_CHECK_EQUAL(maps[2].getRightNeighbour().first, 3);
  BOOST_CHECK_EQUAL(maps[2].getRightNeighbour().second, 10);

  BOOST_CHECK_EQUAL(maps[3].getRightNeighbour().first, 4);
  BOOST_CHECK_EQUAL(maps[3].getRightNeighbour().second, 10);
}

BOOST_AUTO_TEST_CASE(MisedAlignTest_1) {
  std::vector<oma::BinaryMap> vec(7,
                                  oma::BinaryMap("", oma::BinaryMap::Bitset()));
  oma::MapsContainer maps(vec);

  maps.addRightNeighbour(1, 2, 10);

  maps.addRightNeighbour(2, 4, 20);

  maps.addRightNeighbour(4, 5, 10);
  maps.addRightNeighbour(5, 6, 10);

  // tested method should resolve 2 -(20)- 4 && 2 -(10)- 3
  // =>
  // 2 -(10)- 3 -(10)- 4
  maps.addRightNeighbour(2, 3, 10);

  BOOST_CHECK_EQUAL(maps[2].getRightNeighbour().first, 3);
  BOOST_CHECK_EQUAL(maps[2].getRightNeighbour().second, 10);

  BOOST_CHECK_EQUAL(maps[3].getRightNeighbour().first, 4);
  BOOST_CHECK_EQUAL(maps[3].getRightNeighbour().second, 10);
}

BOOST_AUTO_TEST_CASE(MisedAlignTest_2) {
  std::vector<oma::BinaryMap> vec(7,
                                  oma::BinaryMap("", oma::BinaryMap::Bitset()));
  oma::MapsContainer maps(vec);

  maps.addRightNeighbour(1, 2, 10);

  maps.addRightNeighbour(2, 4, 20);

  maps.addRightNeighbour(4, 5, 10);
  maps.addRightNeighbour(5, 6, 10);

  // tested method should resolve 2 -(20)- 4 && 3 -(10)- 4
  // =>
  // 2 -(10)- 3 -(10)- 4
  maps.addRightNeighbour(3, 4, 10);

  BOOST_CHECK_EQUAL(maps[2].getRightNeighbour().first, 3);
  BOOST_CHECK_EQUAL(maps[2].getRightNeighbour().second, 10);

  BOOST_CHECK_EQUAL(maps[3].getRightNeighbour().first, 4);
  BOOST_CHECK_EQUAL(maps[3].getRightNeighbour().second, 10);
}

BOOST_AUTO_TEST_CASE(AnotherReferedAlignTest) {
  std::vector<oma::BinaryMap> vec(7,
                                  oma::BinaryMap("", oma::BinaryMap::Bitset()));
  oma::MapsContainer maps(vec);

  maps.addRightNeighbour(1, 2, 10);
  maps.addRightNeighbour(2, 3, 10);
  maps.addRightNeighbour(3, 4, 10);
  maps.addRightNeighbour(4, 5, 10);
  maps.addRightNeighbour(5, 6, 10);

  maps.addRightNeighbour(2, 4, 20);

  BOOST_CHECK_EQUAL(maps[2].getRightNeighbour().first, 3);
  BOOST_CHECK_EQUAL(maps[2].getRightNeighbour().second, 10);

  BOOST_CHECK_EQUAL(maps[3].getRightNeighbour().first, 4);
  BOOST_CHECK_EQUAL(maps[3].getRightNeighbour().second, 10);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
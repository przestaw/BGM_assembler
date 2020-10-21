//
// Created by przemek on 7/16/20.
//
#include <boost/test/unit_test.hpp>
#include <data/maps_container.h>

BOOST_AUTO_TEST_SUITE(OMA_Test_Suite)

BOOST_AUTO_TEST_SUITE(Maps_Container_Test_Suite)

BOOST_AUTO_TEST_CASE(TestReset_NonEmptyVectorAndEmptyContainer) {

  std::string twoStr = "111111111111111111111111111111111111111111111111111111111111"
                       "111111111111111111111111111111111111111111111111110000000000"
                       "0000000000000000000000000000000000000000";

  bgm::BinaryMap two("two", bgm::BinaryMap::Bitset(twoStr));

  bgm::MapsContainer maps;
  std::vector<bgm::BinaryMap> vec = {two, two};

  BOOST_CHECK_EQUAL(maps.size(), 0);

  maps.resetAndReplace(vec);

  BOOST_CHECK_EQUAL(maps.size(), 2);
  BOOST_CHECK_EQUAL(maps.front().getIdentifier(), "two");
}

BOOST_AUTO_TEST_CASE(TestReset_EmptyVectorAndNonEmptyContainer) {

  std::string oneStr = "000000000000000000000000000000000000000000000000001111111111"
                       "111111111111111111111111111111111111111111111111111111111111"
                       "1111111111111111111111111111111111111111";

  bgm::BinaryMap one("one", bgm::BinaryMap::Bitset(oneStr));

  bgm::MapsContainer maps;
  std::vector<bgm::BinaryMap> vec;

  maps.push_back(one);

  BOOST_CHECK_EQUAL(maps.size(), 1);
  BOOST_CHECK_EQUAL(maps.front().getIdentifier(), "one");

  maps.resetAndReplace(vec);

  BOOST_CHECK_EQUAL(maps.size(), 0);
}

BOOST_AUTO_TEST_CASE(TestReset_NonEmptyVectorAndNonEmptyContainer) {

  std::string oneStr = "000000000000000000000000000000000000000000000000001111111111"
                       "111111111111111111111111111111111111111111111111111111111111"
                       "1111111111111111111111111111111111111111";
  std::string twoStr = "111111111111111111111111111111111111111111111111111111111111"
                       "111111111111111111111111111111111111111111111111110000000000"
                       "0000000000000000000000000000000000000000";

  bgm::BinaryMap one("one", bgm::BinaryMap::Bitset(oneStr));
  bgm::BinaryMap two("two", bgm::BinaryMap::Bitset(twoStr));

  bgm::MapsContainer maps;
  std::vector<bgm::BinaryMap> vec = {two, two};

  maps.push_back(one);

  BOOST_CHECK_EQUAL(maps.size(), 1);
  BOOST_CHECK_EQUAL(maps.front().getIdentifier(), "one");

  maps.resetAndReplace(vec);

  BOOST_CHECK_EQUAL(maps.size(), 2);
  BOOST_CHECK_EQUAL(maps.front().getIdentifier(), "two");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

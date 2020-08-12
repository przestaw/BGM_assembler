//
// Created by przemek on 10.08.2020.
//

#include <algorithms/consensus_builder.h>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(OMA_Test_Suite)

BOOST_AUTO_TEST_SUITE(Consensus_Test_Suite)

BOOST_AUTO_TEST_CASE(OneMap_CreatesValidMap) {
  oma::MapsContainer mapsContainer;
  std::string test   = "1010111100001101010001110101";
  std::string mapStr = "1010111100001101010001110101";

  // reversed string to account for direction od constructor
  std::reverse(mapStr.begin(), mapStr.end());

  mapsContainer.emplace_back("map",
                             oma::BinaryMap::Bitset(std::string(mapStr)));
  oma::ConsensusBuilder testFixture(mapsContainer);

  testFixture.runConsensusSingle();

  BOOST_REQUIRE_EQUAL(1, testFixture.getConsensusMaps().size());
  std::string res;
  boost::to_string(testFixture.getConsensusMaps()[0].getMap(), res);

  BOOST_CHECK_EQUAL(res, test);
}

BOOST_AUTO_TEST_CASE(TwoMaps_CreatesValidMap) {
  oma::MapsContainer mapsContainer;
  std::string mapStr  = "1010111100001101010001110101";
  std::string mapStr1 = "10101111000011010100011";
  std::string mapStr2 = "11100001101010001110101";

  std::reverse(mapStr1.begin(), mapStr1.end());
  std::reverse(mapStr2.begin(), mapStr2.end());

  mapsContainer.emplace_back("map1",
                             oma::BinaryMap::Bitset(std::string(mapStr1)));
  mapsContainer.emplace_back("map2",
                             oma::BinaryMap::Bitset(std::string(mapStr2)));
  mapsContainer.addRightNeighbour(0, 1, 5);
  oma::ConsensusBuilder testFixture(mapsContainer);

  testFixture.runConsensusSingle();

  BOOST_REQUIRE_EQUAL(1, testFixture.getConsensusMaps().size());
  std::string res;
  boost::to_string(testFixture.getConsensusMaps()[0].getMap(), res);

  BOOST_CHECK_EQUAL(res, mapStr);
}

BOOST_AUTO_TEST_CASE(ThreeMaps_CreatesValidMap) {
  oma::MapsContainer mapsContainer;

  std::string mapStr  = "1010111100001101010001110101";
  std::string mapStr1 = "10101111000011010100011";
  std::string mapStr2 = "101111000011010100011101";
  std::string mapStr3 = "11100001101010001110101";

  std::reverse(mapStr1.begin(), mapStr1.end());
  std::reverse(mapStr2.begin(), mapStr2.end());
  std::reverse(mapStr3.begin(), mapStr3.end());

  mapsContainer.emplace_back("map1",
                             oma::BinaryMap::Bitset(std::string(mapStr1)));
  mapsContainer.emplace_back("map2",
                             oma::BinaryMap::Bitset(std::string(mapStr2)));
  mapsContainer.emplace_back("map3",
                             oma::BinaryMap::Bitset(std::string(mapStr3)));
  mapsContainer.addRightNeighbour(0, 1, 3);
  mapsContainer.addRightNeighbour(1, 2, 2);
  oma::ConsensusBuilder testFixture(mapsContainer);

  testFixture.runConsensusSingle();

  BOOST_REQUIRE_EQUAL(1, testFixture.getConsensusMaps().size());
  std::string res;
  boost::to_string(testFixture.getConsensusMaps()[0].getMap(), res);

  BOOST_CHECK_EQUAL(res, mapStr);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
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

BOOST_AUTO_TEST_CASE(FourMaps_CreatesValidMap) {
  oma::MapsContainer mapsContainer;

  std::string mapStr = "1000001000011101000101110000000101111000000111010001100"
                       "0101000000000010000000000"
                       "0001000001100000100011000000000000100001000000100000011"
                       "1010000000010001010001010"
                       "0010010010101000000000000010101000010000000000000001010"
                       "0000000000000000100000000"
                       "0000100000000000011000010001101000000110010010000000100"
                       "0010011100001000111000111"
                       "1100011100001110000000000010000111100100010000101001000"
                       "1000001001111000000001001";
  std::string mapStr1 = "100000100001110100010111000000010111100000011101000110"
                        "00101000000000010000000000"
                        "000100000110000010001100000000000010000100000010000001"
                        "11010000000010001010001010";
  std::string mapStr2 = "000100000110000010001100000000000010000100000010000001"
                        "11010000000010001010001010"
                        "001001001010100000000000001010100001000000000000000101"
                        "00000000000000000100000000";
  std::string mapStr3 = "001001001010100000000000001010100001000000000000000101"
                        "00000000000000000100000000"
                        "000010000000000001100001000110100000011001001000000010"
                        "00010011100001000111000111";
  std::string mapStr4 = "000010000000000001100001000110100000011001001000000010"
                        "00010011100001000111000111"
                        "110001110000111000000000001000011110010001000010100100"
                        "01000001001111000000001001";

  std::reverse(mapStr1.begin(), mapStr1.end());
  std::reverse(mapStr2.begin(), mapStr2.end());
  std::reverse(mapStr3.begin(), mapStr3.end());
  std::reverse(mapStr4.begin(), mapStr4.end());

  mapsContainer.emplace_back("map1",
                             oma::BinaryMap::Bitset(std::string(mapStr1)));
  mapsContainer.emplace_back("map2",
                             oma::BinaryMap::Bitset(std::string(mapStr2)));
  mapsContainer.emplace_back("map3",
                             oma::BinaryMap::Bitset(std::string(mapStr3)));
  mapsContainer.emplace_back("map4",
                             oma::BinaryMap::Bitset(std::string(mapStr4)));
  mapsContainer.addRightNeighbour(0, 1, 80);
  mapsContainer.addRightNeighbour(1, 2, 80);
  mapsContainer.addRightNeighbour(2, 3, 80);
  oma::ConsensusBuilder testFixture(mapsContainer);

  testFixture.runConsensusSingle();

  BOOST_REQUIRE_EQUAL(1, testFixture.getConsensusMaps().size());
  std::string res;
  boost::to_string(testFixture.getConsensusMaps()[0].getMap(), res);

  BOOST_CHECK_EQUAL(res, mapStr);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
//
// Created by przemek on 10.08.2020.
//

#include <algorithms/consensus_builder.h>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(BGM_Test_Suite)

BOOST_AUTO_TEST_SUITE(Consensus_Test_Suite)

 BOOST_AUTO_TEST_CASE(OneMap_CreatesValidMap) {
    bgm::MapsContainer mapsContainer;
  std::string test   = "1010111100001101010001110101";
  std::string mapStr = "1010111100001101010001110101";

  // reversed string to account for direction od constructor
  std::reverse(mapStr.begin(), mapStr.end());

  mapsContainer.emplace_back("map",
                             bgm::BinaryMap::Bitset(std::string(mapStr)));
  bgm::ConsensusBuilder testFixture(mapsContainer);

  testFixture.runConsensusSingle();

  BOOST_REQUIRE_EQUAL(1, testFixture.getConsensusMaps().size());
  std::string res;
  boost::to_string(testFixture.getConsensusMaps()[0].getMap(), res);

  BOOST_CHECK_EQUAL(res, test);
}

BOOST_AUTO_TEST_CASE(TwoMaps_CreatesValidMap) {
  bgm::MapsContainer mapsContainer;
  std::string mapStr  = "1010111100001101010001110101";
  std::string mapStr1 = "10101111000011010100011";
  std::string mapStr2 = "11100001101010001110101";

  std::reverse(mapStr1.begin(), mapStr1.end());
  std::reverse(mapStr2.begin(), mapStr2.end());

  mapsContainer.emplace_back("map1", bgm::BinaryMap::Bitset(std::string(mapStr1)));
  mapsContainer.emplace_back("map2", bgm::BinaryMap::Bitset(std::string(mapStr2)));
  mapsContainer.addRightNeighbour(0, bgm::BinaryMap::Neighbour(1, 5));
  bgm::ConsensusBuilder testFixture(mapsContainer);

  testFixture.runConsensusSingle();

  BOOST_REQUIRE_EQUAL(1, testFixture.getConsensusMaps().size());
  std::string res;
  boost::to_string(testFixture.getConsensusMaps()[0].getMap(), res);

  BOOST_CHECK_EQUAL(res, mapStr);
}

BOOST_AUTO_TEST_CASE(ThreeMapsSameDist_CreatesValidMap) {
  bgm::MapsContainer mapsContainer;

  std::string mapStr  = "10101111000011010100011101010";
  std::string mapStr1 = "10101111000011010100011";
  std::string mapStr2 = "101111000011010100011101";
  std::string mapStr3 = "111000011010100011101010";

  std::reverse(mapStr1.begin(), mapStr1.end());
  std::reverse(mapStr2.begin(), mapStr2.end());
  std::reverse(mapStr3.begin(), mapStr3.end());

  mapsContainer.emplace_back("map1", bgm::BinaryMap::Bitset(std::string(mapStr1)));
  mapsContainer.emplace_back("map2", bgm::BinaryMap::Bitset(std::string(mapStr2)));
  mapsContainer.emplace_back("map3", bgm::BinaryMap::Bitset(std::string(mapStr3)));

  mapsContainer.addRightNeighbour(0, bgm::BinaryMap::Neighbour(1, 3));
  mapsContainer.addRightNeighbour(1, bgm::BinaryMap::Neighbour(2, 3));
  bgm::ConsensusBuilder testFixture(mapsContainer);

  testFixture.runConsensusSingle();

  BOOST_REQUIRE_EQUAL(1, testFixture.getConsensusMaps().size());
  std::string res;
  boost::to_string(testFixture.getConsensusMaps()[0].getMap(), res);

  BOOST_CHECK_EQUAL(res, mapStr);
}

BOOST_AUTO_TEST_CASE(ThreeMapsDiffDist_CreatesValidMap_3) {
  bgm::MapsContainer mapsContainer;

  std::string mapStr  = "1010111100001101010001110101";
  std::string mapStr1 = "10101111000011010100011";
  std::string mapStr2 = "0101111000011010100011101";
  std::string mapStr3 = "11100001101010001110101";

  std::reverse(mapStr1.begin(), mapStr1.end());
  std::reverse(mapStr2.begin(), mapStr2.end());
  std::reverse(mapStr3.begin(), mapStr3.end());

  mapsContainer.emplace_back("map1", bgm::BinaryMap::Bitset(std::string(mapStr1)));
  mapsContainer.emplace_back("map2", bgm::BinaryMap::Bitset(std::string(mapStr2)));
  mapsContainer.emplace_back("map3", bgm::BinaryMap::Bitset(std::string(mapStr3)));

  mapsContainer.addRightNeighbour(0, bgm::BinaryMap::Neighbour(1, 3));
  mapsContainer.addRightNeighbour(1, bgm::BinaryMap::Neighbour(2, 2));
  bgm::ConsensusBuilder testFixture(mapsContainer);

  testFixture.runConsensusSingle();

  BOOST_REQUIRE_EQUAL(1, testFixture.getConsensusMaps().size());
  std::string res;
  boost::to_string(testFixture.getConsensusMaps()[0].getMap(), res);

  BOOST_CHECK_EQUAL(res, mapStr);
}

BOOST_AUTO_TEST_CASE(FourMapsSameDist_CreatesValidMap) {
  bgm::MapsContainer mapsContainer;

  std::string mapStr =
      "100000100001110100010111000000010111100000011101000110001010000000000100"
      "000000000001000001100000100011000000000000100001000000100000011101000000"
      "001000101000101000100100101010000000000000101010000100000000000000010100"
      "000000000000000100000000000010000000000001100001000110100000011001001000"
      "000010000100111000010001110001111100011100001110000000000010000111100100"
      "0100001010010001000001001111000000001001";
  std::string mapStr1 = "100000100001110100010111000000010111100000011101000110"
                        "001010000000000100000000000001000001100000100011000000"
                        "0000001000010000001000000111010000000010001010001010";
  std::string mapStr2 = "000100000110000010001100000000000010000100000010000001"
                        "110100000000100010100010100010010010101000000000000010"
                        "1010000100000000000000010100000000000000000100000000";
  std::string mapStr3 = "001001001010100000000000001010100001000000000000000101"
                        "000000000000000001000000000000100000000000011000010001"
                        "1010000001100100100000001000010011100001000111000111";
  std::string mapStr4 = "000010000000000001100001000110100000011001001000000010"
                        "000100111000010001110001111100011100001110000000000010"
                        "0001111001000100001010010001000001001111000000001001";

  std::reverse(mapStr1.begin(), mapStr1.end());
  std::reverse(mapStr2.begin(), mapStr2.end());
  std::reverse(mapStr3.begin(), mapStr3.end());
  std::reverse(mapStr4.begin(), mapStr4.end());

  mapsContainer.emplace_back("map1", bgm::BinaryMap::Bitset(std::string(mapStr1)));
  mapsContainer.emplace_back("map2", bgm::BinaryMap::Bitset(std::string(mapStr2)));
  mapsContainer.emplace_back("map3", bgm::BinaryMap::Bitset(std::string(mapStr3)));
  mapsContainer.emplace_back("map4", bgm::BinaryMap::Bitset(std::string(mapStr4)));
  mapsContainer.addRightNeighbour(0, bgm::BinaryMap::Neighbour(1, 80));
  mapsContainer.addRightNeighbour(1, bgm::BinaryMap::Neighbour(2, 80));
  mapsContainer.addRightNeighbour(2, bgm::BinaryMap::Neighbour(3, 80));
  bgm::ConsensusBuilder testFixture(mapsContainer);

  testFixture.runConsensusSingle();

  BOOST_REQUIRE_EQUAL(1, testFixture.getConsensusMaps().size());
  std::string res;
  boost::to_string(testFixture.getConsensusMaps()[0].getMap(), res);

  BOOST_CHECK_EQUAL(res, mapStr);
}

BOOST_AUTO_TEST_CASE(FourMapsDiffDist_CreatesValidMap) {
  bgm::MapsContainer mapsContainer;

  std::string mapStr =
      "100000100001110100010111000000010111100000011101000110001010000000000100"
      "000000000001000001100000100011000000000000100001000000100000011101000000"
      "001000101000101000100100101010000000000000101010000100000000000000010100"
      "000000000000000100000000000010000000000001100001000110100000011001001000"
      "000010000100111000010001110001111100011100001110000000000010000111100100"
      "0100001010010001000001001111000000001001";
  std::string mapStr1 = "100000100001110100010111000000010111100000011101000110"
                        "001010000000000100000000000001000001100000100011000000"
                        "000000100001000000100000011101000000001000101000101";
  std::string mapStr2 = "000100000110000010001100000000000010000100000010000001"
                        "110100000000100010100010100010010010101000000000000010"
                        "101000010000000000000001010000000000000000";
  std::string mapStr3 = "001001001010100000000000001010100001000000000000000101"
                        "000000000000000001000000000000100000000000011000010001"
                        "1010000001100100100000001000010011100001000111000";
  std::string mapStr4 = "000010000000000001100001000110100000011001001000000010"
                        "000100111000010001110001111100011100001110000000000010"
                        "0001111001000100001010010001000001001111000000001001";

  std::reverse(mapStr1.begin(), mapStr1.end());
  std::reverse(mapStr2.begin(), mapStr2.end());
  std::reverse(mapStr3.begin(), mapStr3.end());
  std::reverse(mapStr4.begin(), mapStr4.end());

  mapsContainer.emplace_back("map1", bgm::BinaryMap::Bitset(std::string(mapStr1)));
  mapsContainer.emplace_back("map2", bgm::BinaryMap::Bitset(std::string(mapStr2)));
  mapsContainer.emplace_back("map3", bgm::BinaryMap::Bitset(std::string(mapStr3)));
  mapsContainer.emplace_back("map4", bgm::BinaryMap::Bitset(std::string(mapStr4)));

  mapsContainer.addRightNeighbour(0, bgm::BinaryMap::Neighbour(1, 71));
  mapsContainer.addRightNeighbour(1, bgm::BinaryMap::Neighbour(2, 87));
  mapsContainer.addRightNeighbour(2, bgm::BinaryMap::Neighbour(3, 83));

  bgm::ConsensusBuilder testFixture(mapsContainer);

  testFixture.runConsensusSingle();

  BOOST_REQUIRE_EQUAL(1, testFixture.getConsensusMaps().size());
  std::string res;
  boost::to_string(testFixture.getConsensusMaps()[0].getMap(), res);

  BOOST_CHECK_EQUAL(res, mapStr);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
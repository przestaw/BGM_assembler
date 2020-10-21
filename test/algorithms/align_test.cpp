//
// Created by przemek on 7/16/20.
//

#include <algorithms/align_algorithm.h>
#include <boost/test/unit_test.hpp>
#include <data/bin_map.h>

BOOST_AUTO_TEST_SUITE(OMA_Test_Suite)

BOOST_AUTO_TEST_SUITE(Align_Test_Suite)

BOOST_AUTO_TEST_CASE(TwoPerfectlyMatchedMaps_AreAligned) {

  std::string oneStr = "000000000000000000000000000000000000000000000000001111111111"
                       "111111111111111111111111111111111111111111111111111111111111"
                       "1111111111111111111111111111111111111111";
  std::string twoStr = "111111111111111111111111111111111111111111111111111111111111"
                       "111111111111111111111111111111111111111111111111110000000000"
                       "0000000000000000000000000000000000000000";

  std::reverse(oneStr.begin(), oneStr.end());
  std::reverse(twoStr.begin(), twoStr.end());

  bgm::BinaryMap::Bitset one(oneStr);
  bgm::BinaryMap::Bitset two(twoStr);

  bgm::MapsContainer maps; // for compatibility
  bgm::AlignAlgorithm aligner(maps, 40);

  bgm::AlignAlgorithm::Alignment res = aligner.align(two, one);

  // shift
  BOOST_CHECK_EQUAL(res.first, 50);
  // perfect match
  BOOST_CHECK_EQUAL(res.second, 0);
}

BOOST_AUTO_TEST_CASE(TwoAllDifferentMaps_AreNotAligned) {
  bgm::BinaryMap::Bitset one(
      std::string("1111111111"
                  "111111111111111111111111111111111111111111111111111111111111"
                  "1111111111111111111111111111111111111111"));
  bgm::BinaryMap::Bitset two(
      std::string("000000000000000000000000000000000000000000000000000000000000"
                  "0000000000000000000000000000000000000000"));

  bgm::MapsContainer maps; // for compatibility
  bgm::AlignAlgorithm aligner(maps, 10);

  bgm::AlignAlgorithm::Alignment res = aligner.align(two, one);

  // shift 0 means no match
  BOOST_CHECK_EQUAL(res.first, -1);
}

BOOST_AUTO_TEST_CASE(TwoDiffrentlySizedMaps_AreAligned_1) {
  std::string oneStr = "0000000000000111111111111111111111111111111111111111111"
                       "1111111111111111111111111111111111111111111111111111111"
                       "1111111111111";
  std::string twoStr = "0011111111111111111111111111111111111111111111111111111"
                       "1111111111111111111111111111111111111111111111111111111"
                       "1100000000000000000000000000000000000000000000000000";

  std::reverse(oneStr.begin(), oneStr.end());
  std::reverse(twoStr.begin(), twoStr.end());

  bgm::BinaryMap::Bitset one(oneStr);
  bgm::BinaryMap::Bitset two(twoStr);
  bgm::MapsContainer maps; // for compatibility
  bgm::AlignAlgorithm aligner(maps, 40);

  bgm::AlignAlgorithm::Alignment res1 = aligner.align(two, one);

  // shift
  BOOST_CHECK_EQUAL(res1.first, 50);
  // perfect match
  BOOST_CHECK_EQUAL(res1.second, 0);
}

BOOST_AUTO_TEST_CASE(TwoDiffrentlySizedMaps_AreAligned_2) {
  std::string oneStr =
      "00000000000001111111111"
      "111111111111111111111111111111111111111111111111111111111111"
      "111111111111111111111111111111111111111100000000000000000000000000000000"
      "0000000000000000001";
  std::string twoStr = "00111111111111111111111111111111111111111111111111111111111111"
                       "111111111111111111111111111111111111111111111111110000000000"
                       "0000000000000000000000000000000000000000";

  std::reverse(oneStr.begin(), oneStr.end());
  std::reverse(twoStr.begin(), twoStr.end());

  bgm::BinaryMap::Bitset one(oneStr);
  bgm::BinaryMap::Bitset two(twoStr);
  bgm::MapsContainer maps; // for compatibility
  bgm::AlignAlgorithm aligner(maps, 40);

  bgm::AlignAlgorithm::Alignment res1 = aligner.align(one, two);

  // shift
  BOOST_CHECK_EQUAL(res1.first, 1);
  // perfect match
  BOOST_CHECK_EQUAL(res1.second, 0);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
//
// Created by przemek on 7/16/20.
//

#include <algorithms/align_algorithm.h>
#include <boost/test/unit_test.hpp>
#include <data/bin_map.h>

BOOST_AUTO_TEST_SUITE(BGM_Test_Suite)

BOOST_AUTO_TEST_SUITE(Align_Test_Suite)

BOOST_AUTO_TEST_SUITE(Align_From_Ends_Test_Suite)

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

  bgm::AlignAlgorithm::LayoutAlign res = aligner.align(two, one);

  // shift
  BOOST_CHECK_EQUAL(res.getShift(), 50);
  // perfect match
  BOOST_CHECK_EQUAL(res.getQuality(), 0);
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

  bgm::AlignAlgorithm::LayoutAlign res = aligner.align(two, one);

  // means no match
  BOOST_CHECK_EQUAL(res.isValid(), false);
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

  bgm::AlignAlgorithm::LayoutAlign res1 = aligner.align(two, one);

  // shift
  BOOST_CHECK_EQUAL(res1.getShift(), 50);
  // perfect match
  BOOST_CHECK_EQUAL(res1.getQuality(), 0);
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

  bgm::AlignAlgorithm::LayoutAlign res1 = aligner.align(one, two);

  // shift
  BOOST_CHECK_EQUAL(res1.getShift(), 1);
  // perfect match
  BOOST_CHECK_EQUAL(res1.getQuality(), 0);
}
BOOST_AUTO_TEST_SUITE_END()

/// new suite
BOOST_AUTO_TEST_SUITE(Align_From_Begins_Test_Suite)

BOOST_AUTO_TEST_CASE(Test_Align_2_Binmaps_1) {
  bgm::BinaryMap map1 = bgm::BinaryMap(
      "one",
      "00000000000000000000000000000000000000000000000000111111111111111111111111111111"
      "11111111111111111111111111111111111111111111111111111111111111111111111111111111");
  bgm::BinaryMap map2 = bgm::BinaryMap(
      "two",
      "11111111111111111111111111111111111111111111111111111111111111111111111111111111"
      "11111111111111111111111111111100000000000000000000000000000000000000000000000000");

  auto res = bgm::AlignAlgorithm::pyAlignFromBegins(map1.getMap(), map2.getMap());

  auto expected = bgm::VisualAlignment(50, 110, 0);

  BOOST_CHECK_EQUAL(res, expected);
}

BOOST_AUTO_TEST_CASE(Test_Align_2_Binmaps_diff_Size_1) {
  bgm::BinaryMap map1 = bgm::BinaryMap(
      "one",
      "0000000000000111111111111111111111111111111111111111111111111111111111111111111"
      "11111111111111111111111111111111111111111111");
  bgm::BinaryMap map2 = bgm::BinaryMap(
      "two",
      "0011111111111111111111111111111111111111111111111111111111111111111111111111111"
      "1111111111111111111111111111111110000000000000000000000000000000000000000000000");

  auto res = bgm::AlignAlgorithm::pyAlignFromBegins(map1.getMap(), map2.getMap());

  auto expected = bgm::VisualAlignment(11, 112, 0);

  BOOST_CHECK_EQUAL(res, expected);
}

BOOST_AUTO_TEST_CASE(Test_Align_2_Binmaps_diff_Size_2) {
  bgm::BinaryMap map1 = bgm::BinaryMap(
      "one",
      "111100000000000001111111111111111111111111111111111111111111111111111111111111111"
      "1111111111111111111111111111111111111111111111");
  bgm::BinaryMap map2 = bgm::BinaryMap(
      "two",
      "00111111111111111111111111111111111111111111111111111111111111111111111111111111"
      "11111111111111111111111111111111000000000000000000000000000000000000000000000000"
      "00");

  auto res = bgm::AlignAlgorithm::pyAlignFromBegins(map1.getMap(), map2.getMap());

  auto expected = bgm::VisualAlignment(15, 112, 0);

  BOOST_CHECK_EQUAL(res, expected);
}

BOOST_AUTO_TEST_CASE(Test_Align_2_Binmaps_diff_Size_3) {
  bgm::BinaryMap map1 = bgm::BinaryMap(
      "one",
      "000000001111111111111111111110111111111111111111111110000000111111111111111111111"
      "111111111111111111111111111111111111100000000000000000000000000111000000000000000"
      "0000001");
  bgm::BinaryMap map2 = bgm::BinaryMap(
      "two",
      "111111111011111111111111111111111000000011111111111111111111111111111111111111111"
      "11111111111111111000000000000000000000000001110000000");

  auto res = bgm::AlignAlgorithm::pyAlignFromBegins(map1.getMap(), map2.getMap());

  auto expected = bgm::VisualAlignment(20, 134, 0);

  BOOST_CHECK_EQUAL(res, expected);
}

BOOST_AUTO_TEST_CASE(Test_Align_2_Binmaps_1_inv) {
  // inverted arguments - negative result expected
  bgm::BinaryMap map1 = bgm::BinaryMap(
      "one",
      "00000000000000000000000000000000000000000000000000111111111111111111111111111111"
      "11111111111111111111111111111111111111111111111111111111111111111111111111111111");
  bgm::BinaryMap map2 = bgm::BinaryMap(
      "two",
      "11111111111111111111111111111111111111111111111111111111111111111111111111111111"
      "11111111111111111111111111111100000000000000000000000000000000000000000000000000");

  auto res = bgm::AlignAlgorithm::pyAlignFromBegins(map2.getMap(), map1.getMap());

  auto expected = bgm::VisualAlignment(-50, 110, 0);

  BOOST_CHECK_EQUAL(res, expected);
}

BOOST_AUTO_TEST_CASE(Test_Align_2_Binmaps_diff_Size_2_inv) {
  // inverted arguments - negative result expected
  bgm::BinaryMap map1 = bgm::BinaryMap(
      "one",
      "111100000000000001111111111111111111111111111111111111111111111111111111111111111"
      "1111111111111111111111111111111111111111111111");
  bgm::BinaryMap map2 = bgm::BinaryMap(
      "two",
      "00111111111111111111111111111111111111111111111111111111111111111111111111111111"
      "11111111111111111111111111111111000000000000000000000000000000000000000000000000"
      "00");

  auto res = bgm::AlignAlgorithm::pyAlignFromBegins(map2.getMap(), map1.getMap());

  auto expected = bgm::VisualAlignment(-15, 112, 0);

  BOOST_CHECK_EQUAL(res, expected);
}

BOOST_AUTO_TEST_CASE(Test_Align_2_Binmaps_diff_Size_3_inv) {
  // inverted arguments - negative result expected
  bgm::BinaryMap map1 = bgm::BinaryMap(
      "one",
      "000000001111111111111111111110111111111111111111111110000000111111111111111111111"
      "111111111111111111111111111111111111100000000000000000000000000111000000000000000"
      "0000001");
  bgm::BinaryMap map2 = bgm::BinaryMap(
      "two",
      "111111111011111111111111111111111000000011111111111111111111111111111111111111111"
      "11111111111111111000000000000000000000000001110000000");

  auto res = bgm::AlignAlgorithm::pyAlignFromBegins(map2.getMap(), map1.getMap());

  auto expected = bgm::VisualAlignment(-20, 134, 0);

  BOOST_CHECK_EQUAL(res, expected);
}

BOOST_AUTO_TEST_CASE(Test_Align_2_Binmaps_With_Diff) {
  bgm::BinaryMap map1 = bgm::BinaryMap(
      "one",
      "00000000000000000000000000000000000000000000000000111111111111111111111111111111"
      "11111111111111111111111111111111111111111111111111111111111111111111111100011111");
  bgm::BinaryMap map2 = bgm::BinaryMap(
      "two",
      "11111111111111111111111111111110000111111111111111111111111111111111111111111111"
      "11111111111111111111111111111100000000000000000000000000000000000000000000000000");

  auto res = bgm::AlignAlgorithm::pyAlignFromBegins(map1.getMap(), map2.getMap());

  auto expected = bgm::VisualAlignment(50, 110, 7);

  BOOST_CHECK_EQUAL(res, expected);
}

BOOST_AUTO_TEST_CASE(Test_Align_2_Binmaps_diff_Size_1_With_Diff) {
  bgm::BinaryMap map1 = bgm::BinaryMap(
      "one",
      "1000111111111111111111111111111111111111111111111111111111111110000011111111111111"
      "11111111000111111110011011100111000000000000000000000000000000000000000000000000");
  bgm::BinaryMap map2 =
      bgm::BinaryMap("two",
                     "1111111111111111111111111111111000011111111111111111111111111111111"
                     "1111111111111111111111111111111100000000000000000000000000000");

  auto res = bgm::AlignAlgorithm::pyAlignFromBegins(map1.getMap(), map2.getMap());

  auto expected = bgm::VisualAlignment(15, 128, 17);

  BOOST_CHECK_EQUAL(res, expected);
}

BOOST_AUTO_TEST_CASE(Test_Align_2_Binmaps_Diff_Size_2_With_Diff) {
  bgm::BinaryMap map1 =
      bgm::BinaryMap("one",
                     "0000000011111111111111111111101111111111111111111111100000001111111"
                     "1110000111111101111111111111111111111111111111111110000000000000000"
                     "00000000001110000000000000000000001");
  bgm::BinaryMap map2 = bgm::BinaryMap(
      "two",
      "1111111110111111111111111111111110000000111111110000111111111110000111111111111111"
      "1111111111111111000000000000000000000000001010000000");

  auto res = bgm::AlignAlgorithm::pyAlignFromBegins(map1.getMap(), map2.getMap());

  auto expected = bgm::VisualAlignment(20, 134, 10);

  BOOST_CHECK_EQUAL(res, expected);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
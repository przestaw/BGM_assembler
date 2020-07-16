//
// Created by przemek on 7/16/20.
//

#include <align_algorithm.h>
#include <boost/test/unit_test.hpp>
#include <data/bin_map.h>

BOOST_AUTO_TEST_SUITE(OMA_Test_Suite)

BOOST_AUTO_TEST_SUITE(Align_Test_Suite)

BOOST_AUTO_TEST_CASE(TwoPerfectlyMatchedMaps_AreAligned) {
  oma::BinaryMap::Bitset one(
      std::string("000000000000000000000000000000000000000000000000001111111111"
                  "111111111111111111111111111111111111111111111111111111111111"
                  "1111111111111111111111111111111111111111"));
  oma::BinaryMap::Bitset two(
      std::string("111111111111111111111111111111111111111111111111111111111111"
                  "111111111111111111111111111111111111111111111111110000000000"
                  "0000000000000000000000000000000000000000"));

  oma::AlignAlgorithm aligner(100);

  oma::AlignAlgorithm::Alignment res = aligner.align(two, one);

  // shift
  BOOST_CHECK_EQUAL(res.first, 50);
  // perfect match
  BOOST_CHECK_EQUAL(res.second, 0);
}

BOOST_AUTO_TEST_CASE(TwoAllDifferentMaps_AreNotAligned) {
  oma::BinaryMap::Bitset one(
      std::string("1111111111"
                  "111111111111111111111111111111111111111111111111111111111111"
                  "1111111111111111111111111111111111111111"));
  oma::BinaryMap::Bitset two(
      std::string("000000000000000000000000000000000000000000000000000000000000"
                  "0000000000000000000000000000000000000000"));

  oma::AlignAlgorithm aligner(100);

  oma::AlignAlgorithm::Alignment res = aligner.align(two, one);

  // shift 0 means no match
  BOOST_CHECK_EQUAL(res.first, 0);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
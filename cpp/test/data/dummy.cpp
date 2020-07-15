//
// Created by przemek on 7/15/20.
//

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Project_Test_Suite)

BOOST_AUTO_TEST_SUITE(Module_Test_Suite)

BOOST_AUTO_TEST_CASE(DummyTest_Passes) {
  BOOST_CHECK_EQUAL(1, 1);
  BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
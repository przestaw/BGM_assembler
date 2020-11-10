//
// Created by przestaw on 27.10.2020.
//

#include <boost/test/unit_test.hpp>
#include <task/assembly_task.h>

BOOST_AUTO_TEST_SUITE(BGM_Test_Suite)

BOOST_AUTO_TEST_SUITE(Assembly_Task_Test_Suite)

BOOST_AUTO_TEST_CASE(TestTask_3MapsAssembly) {
  bgm::AssemblyParams params(1, 10, 0.5, 0.1, 1, true, false);

  std::string mapStr  = "1010111100001101010001110101";
  std::string mapStr1 = "10101111000011010100011";
  std::string mapStr2 = "0101111000011010100011101";
  std::string mapStr3 = "11100001101010001110101";

  bgm::AssemblyTask task({bgm::BinaryMap("map1", mapStr1),
                          bgm::BinaryMap("map2", mapStr2),
                          bgm::BinaryMap("map3", mapStr3)},
                         params);

  task.execute();

  BOOST_REQUIRE_EQUAL(1, task.getResult().size());
  std::string res;
  boost::to_string(task.getResult()[0].getMap(), res);

  BOOST_CHECK_EQUAL(res, mapStr);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
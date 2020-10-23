//
// Created by przestaw on 20.10.2020.
//
#include <boost/python.hpp>
#include <task/py_assembly_task.h>

class testClas {
public:
  const bgm::BinaryMap& getMap() const { return map; }

private:
  bgm::BinaryMap map = bgm::BinaryMap("ja", "111000111000");
};

BOOST_PYTHON_MODULE(BGM_asm) {
  using namespace boost::python;

  // class AssemblyParams with constructor
  // note : signature : unsigned int, unsigned int, double, double, unsigned, bool, bool
  class_<bgm::AssemblyTask::Params>(
      "AssemblyParams",
      init<unsigned int, unsigned int, double, double, unsigned, bool, bool>())
      // properties for class AssemblyParams
      .def_readonly("threads", &bgm::AssemblyParams::getThreads)
      .def_readonly("minOverlap", &bgm::AssemblyParams::getMinOverlap)
      .def_readonly("minPart", &bgm::AssemblyParams::getMinPart)
      .def_readonly("threshold", &bgm::AssemblyParams::getThreshold)
      .def_readonly("repeats", &bgm::AssemblyParams::getRepeats)
      .def_readonly("threshold", &bgm::AssemblyParams::getThreshold)
      .def_readonly("markerPreference", &bgm::AssemblyParams::isMarkerPreference)
      .def_readonly("reuseMaps", &bgm::AssemblyParams::isReuseMaps);

  // class BinaryMap
  class_<bgm::BinaryMap>("CppBinaryMap", init<std::string, std::string>())
      .add_property(
          "name", &bgm::BinaryMap::getCIdentifier, &bgm::BinaryMap::setIdentifier)
      .def("toFastaRecord", &bgm::BinaryMap::toFastaRecord)
      //.add_property("markFlag", &bgm::BinaryMap::isMark, &bgm::BinaryMap::setMark)
      .add_property("contigFlag", &bgm::BinaryMap::isContig, &bgm::BinaryMap::setContig);

  // class AssemblyTask
  class_<bgm::PyAssemblyTask, bases<>, bgm::PyAssemblyTask*, boost::noncopyable>(
      "AssemblyTask", init<bgm::PyAssemblyTask::Params>())
      .def("addMap", &bgm::PyAssemblyTask::addMap)
      .def("execute", &bgm::PyAssemblyTask::execute)
      .def("getResult", &bgm::PyAssemblyTask::getPyResult);
}
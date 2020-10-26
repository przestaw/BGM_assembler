//
// Created by przestaw on 20.10.2020.
//
#include <boost/python/args.hpp>
#include <boost/python/class.hpp>
#include <boost/python/def.hpp>
#include <boost/python/module.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/raw_function.hpp>
#include <boost/python/tuple.hpp>
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
  class_<bgm::PyAssemblyTask::Params>(
      "AssemblyParams",
      init<unsigned int, unsigned int, double, double, unsigned, bool, bool>(
          args("self",
               "threads",
               "minOverlap",
               "minPart",
               "threshold",
               "repeats",
               "markerPreference",
               "reuseMaps")))
      // properties for class AssemblyParams
      .def_readonly("threads",
                    &bgm::AssemblyParams::getThreads,
                    "No of threads, 0 will enable all available on the machine, other "
                    "values will create demanded no of threads")
      .def_readonly("minOverlap",
                    &bgm::AssemblyParams::getMinOverlap,
                    "minimum overlap length to be considered")
      .def_readonly("minPart",
                    &bgm::AssemblyParams::getMinPart,
                    "minimum overlap part of smaller map")
      .def_readonly("repeats",
                    &bgm::AssemblyParams::getRepeats,
                    "no of folds to perform before output")
      .def_readonly("threshold",
                    &bgm::AssemblyParams::getThreshold,
                    "max no of differences for overlap to be meaningful")
      .def_readonly("markerPreference",
                    &bgm::AssemblyParams::isMarkerPreference,
                    "preference of marker or empty space")
      .def_readonly("reuseMaps",
                    &bgm::AssemblyParams::isReuseMaps,
                    "reuse all maps after each fold to pile up rmaps on contigs");

  // class BinaryMap
  class_<bgm::BinaryMap>(
      "CppBinaryMap",
      init<std::string, std::string>(
          args("self", "name", "binmap_str"),
          "Wrapper for bitset representation of a binary map sequence"))
      .add_property("name",
                    &bgm::BinaryMap::getCIdentifier,
                    &bgm::BinaryMap::setIdentifier,
                    "sequence identifier")
      .def("toFastaRecord",
           &bgm::BinaryMap::toFastaRecord,
           "converts map to FASTA-style formated string")
      //.add_property("markFlag", &bgm::BinaryMap::isMark, &bgm::BinaryMap::setMark)
      .add_property(
          "contigFlag",
          &bgm::BinaryMap::isContig,
          &bgm::BinaryMap::setContig,
          "contig flag - indicates that map origin is from performed consensus");

  // class AssemblyTask
  class_<bgm::PyAssemblyTask, bases<>, bgm::PyAssemblyTask*, boost::noncopyable>(
      "AssemblyTask", init<bgm::PyAssemblyTask::Params>(args("self", "params")))
      .def("addMap", &bgm::PyAssemblyTask::addMap, "adds map to internal container")
      .def("execute",
           &bgm::PyAssemblyTask::execute,
           "runs algorithm with params specified in constructor")
      .def("getResult",
           &bgm::PyAssemblyTask::getPyResult,
           "obtains resulting binary maps");
}
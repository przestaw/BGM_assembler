//
// Created by przestaw on 20.10.2020.
//
#include "python_compare_bitmaps.h"
#include <boost/python/args.hpp>
#include <boost/python/class.hpp>
#include <boost/python/def.hpp>
#include <boost/python/module.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/raw_function.hpp>
#include <boost/python/tuple.hpp>
#include <python_functions.h>
#include <task/py_assembly_task.h>
#include <utility>

namespace p = boost::python;
namespace np = boost::python::numpy;

BOOST_PYTHON_MODULE(BGM_asm) {
  using namespace boost::python;

  // class AssemblyParams with constructor
  // note : signature : unsigned int, unsigned int, double, double, unsigned, bool, bool
  class_<bgm::py::PyAssemblyTask::Params>(
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
                    &bgm::BinaryMap::getPyIdentifier,
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

  // xor between 2 maps
  def("xorBinaryMaps",
      &bgm::xorBinaryMaps,
      "Performs xor between maps and return diffrences count");

  // class AssemblyTask
  class_<bgm::py::PyAssemblyTask, bases<>, bgm::py::PyAssemblyTask*, boost::noncopyable>(
      "AssemblyTask", init<bgm::py::PyAssemblyTask::Params>(args("self", "params")))
      .def("addMap", &bgm::py::PyAssemblyTask::addMap, "adds map to internal container")
      .def("execute",
           &bgm::py::PyAssemblyTask::execute,
           "runs algorithm with params specified in constructor")
      .def("getResult",
           &bgm::py::PyAssemblyTask::getPyResult,
           "obtains resulting binary maps");

  // find best align using bgm
  def("findBestMapAlignemnt",
      &bgm::findBestAlignemnt,
      "Finds overlap with the least diffrences between maps");

  np::initialize();

  def("alignMapToRef",
      &bgm::alignMapToRef<int>,
      "Aligns map to referenc and returns image ndarray");
}
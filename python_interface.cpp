//
// Created by przestaw on 20.10.2020.
//

#include <boost/python/args.hpp>
#include <boost/python/class.hpp>
#include <boost/python/def.hpp>
#include <boost/python/module.hpp>
#include <boost/python/numpy.hpp>
#include <compare_bitmaps.h>
#include <error/exeception.h>
#include <free_functions.h>
#include <multi_compare_bitmaps.h>
#include <py_assembly_task.h>

namespace p  = boost::python;
namespace np = boost::python::numpy;

void translateException(bgm::error::MismatchedArgumentsSize const& e){
  // Use the Python 'C' API to set up an exception object
  PyErr_SetString(PyExc_ValueError, e.what());
}

BOOST_PYTHON_MODULE(_bgm_module) {
  using namespace boost::python;
  register_exception_translator<bgm::error::MismatchedArgumentsSize>(&translateException);

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
      .def("__len__", bgm::py::lengthOfBinaryMap)
      .def("copy", bgm::py::copyBinaryMap, "Returns deepcopy of a map")
      .def("__str__", &bgm::BinaryMap::toFastaRecord)
      .def("toFastaRecord",
           &bgm::BinaryMap::toFastaRecord,
           "converts map to FASTA-style formated string")
      .def("xor",
           &bgm::py::xorBinaryMaps,
           "Performs xor of self and other and returns new BinaryMap")
      .def("xor_count",
           &bgm::py::xorCountBinaryMaps,
           "Performs xor of self and other and returns count of 1")
      .add_property(
          "string_map", &bgm::BinaryMap::toStringBMap, "String version of binary map")
      .def("count", &bgm::BinaryMap::count, (arg("symbol") = true), "Counts symbols in map")
      .add_property("contigFlag",
                    &bgm::BinaryMap::isContig,
                    &bgm::BinaryMap::setContig,
                    "contig flag - indicates that map origin is from performed consensus")
      //.add_property("markFlag", &bgm::BinaryMap::isMark, &bgm::BinaryMap::setMark)
      ;

  // xor count between 2 maps
  def("xorCountBinaryMaps",
      &bgm::py::xorCountBinaryMaps,
      "Performs xor between maps and return diffrences count");

  // xor between 2 maps
  def("xorBinaryMaps",
      &bgm::py::xorBinaryMaps,
      "Performs xor between maps and return difference map");

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
  def("findBestShift",
      &bgm::py::findBestShift,
      "Finds overlap with the least diffrences between maps");

  // find best align using bgm
  def("findBestAlignemnt",
      &bgm::py::findBestAlignemnt,
      "Finds overlap with the least diffrences between maps");

  // initialize numpy
  np::initialize();

  // class CompareBitmaps
  class_<bgm::py::CompareBitmaps, bases<>, bgm::py::CompareBitmaps*, boost::noncopyable>(
      "CompareBitmaps", init<bgm::BinaryMap, bgm::BinaryMap>((arg("ref"), arg("map"))))
      .def("compareInt",
           &bgm::py::CompareBitmaps::alignMapToRefPy<uint8_t>,
           "compares binary maps")
      .def("compareFloat",
           &bgm::py::CompareBitmaps::alignMapToRefPy<float>,
           "compares binary maps");

  // class MultiCompareBitmaps
  class_<bgm::py::MultiCompareBitmaps,
         bases<>,
         bgm::py::MultiCompareBitmaps*,
         boost::noncopyable>("MultiCompareBitmaps", init<bgm::BinaryMap>((arg("ref"))))
      .def("addMapToCompare",
           &bgm::py::MultiCompareBitmaps::addMap,
           "add map for comparison")
      .def("compareInt",
           &bgm::py::MultiCompareBitmaps::alignMapToRefPy<int8_t>,
           "compares binary maps")
      .def("compareFloat",
           &bgm::py::MultiCompareBitmaps::alignMapToRefPy<float>,
           "compares binary maps");
}
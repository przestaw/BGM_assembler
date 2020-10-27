//
// Created by przestaw on 23.10.2020.
//

#include <data/result_writer.hpp>
#include <task/py_assembly_task.h>

using namespace bgm;
using namespace bgm::py;

void PyAssemblyTask::addMap(BinaryMap map) {
  // lock to do not interfere with execution
  std::unique_lock lock(mapsGuard);
  maps_.push_back(std::move(map));
}

void PyAssemblyTask::operator()(mt4cpp::Progress& p) {
  // lock to prevent from adding new maps during run
  std::unique_lock lock(mapsGuard);
  // same principles
  AssemblyTask::operator()(p);
}

boost::python::list PyAssemblyTask::getPyResult() {
  // result must be returned as python list
  boost::python::list list;
  // reuse code from AssemblyTask
  for (const auto& map : AssemblyTask::getResult()) { list.append(map); }
  return list;
}

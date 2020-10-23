//
// Created by przestaw on 23.10.2020.
//

#include <task/py_assembly_task.h>

using namespace bgm;

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
  auto result = AssemblyTask::getResult();
  boost::python::list ret(result);
  return ret;
}

//
// Created by przestaw on 23.10.2020.
//

#ifndef BGM_ASM_PY_ASSEMBLY_TASK_H
#define BGM_ASM_PY_ASSEMBLY_TASK_H

#include <boost/python.hpp>
#include <task/assembly_task.h>

namespace bgm::py {

  class PyAssemblyTask : public bgm::AssemblyTask {
  public:
    using Params = bgm::AbstractTask::Params;

    explicit PyAssemblyTask(Params params) : AssemblyTask(std::vector<bgm::BinaryMap>(), params) {}

    void addMap(bgm::BinaryMap map);

    boost::python::list getPyResult();

  protected:
    // unnecessary
    double profileAlgorithm() override { return 0.0; };
  private:
    void operator()(mt4cpp::Progress& p) override;

    std::mutex mapsGuard;
  };
} // namespace bgm

#endif // BGM_ASM_PY_ASSEMBLY_TASK_H

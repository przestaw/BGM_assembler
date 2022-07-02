//
// Created by przestaw on 23.10.2020.
//

#ifndef BGM_ASM_PY_ASSEMBLY_TASK_H
#define BGM_ASM_PY_ASSEMBLY_TASK_H

#include <boost/python.hpp>
#include <task/assembly_task.h>

namespace bgm::py {

  /**
   * Python-facing wrapper around AssemblyTask exposing map addition and list results
   */
  class PyAssemblyTask : public bgm::AssemblyTask {
  public:
    using Params = bgm::AbstractTask::Params;

    /**
     * Constructor
     * @param params assembly parameters
     */
    explicit PyAssemblyTask(Params params)
        : AssemblyTask(std::vector<bgm::BinaryMap>(), params) {}

    /**
     * Adds a binary map (and its reversed counterpart) to the task container
     * @param map binary map to add
     */
    void addMap(bgm::BinaryMap map);

    /**
     * Returns assembled consensus maps as a Python list of CppBinaryMap objects
     * @return Python list of resulting maps
     */
    boost::python::list getPyResult();

  protected:
    // unnecessary
    double profileAlgorithm() override { return 0.0; };

  private:
    void operator()(mt4cpp::Progress& p) override;

    mutable std::mutex mapsGuard;
  };
} // namespace bgm::py

#endif // BGM_ASM_PY_ASSEMBLY_TASK_H

//
// Created by przestaw on 21.10.2020.
//

#ifndef BGM_ASM_ASSEMBLY_TASK_H
#define BGM_ASM_ASSEMBLY_TASK_H

#include <common/assembly_params.h>
#include <task/abstract_task.h>
#include <utility>
#include <vector>

namespace bgm {

  class AssemblyTask : public AbstractTask {
  public:
    using Params = AbstractTask::Params;

    /**
     * Constructos
     * @param params parameter for task
     */
    AssemblyTask(std::vector<BinaryMap> maps, Params params);

    std::vector<BinaryMap> getResult();

  private:
    void operator()(mt4cpp::Progress& p) override;

  protected:
    double profileAlgorithm() override;
  };

} // namespace bgm

#endif // BGM_ASM_ASSEMBLY_TASK_H

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

  /**
   * In-memory assembly task executing alignment and consensus folds on a vector of BinaryMaps
   */
  class AssemblyTask : public AbstractTask {
  public:
    using Params = AbstractTask::Params;

    /**
     * Constructor
     * @param maps input binary maps
     * @param params parameters for assembly task
     */
    AssemblyTask(std::vector<BinaryMap> maps, Params params);

    /**
     * Returns assembled consensus binary maps
     * @return vector of resulting BinaryMaps
     */
    std::vector<BinaryMap> getResult();

  protected:
    void operator()(mt4cpp::Progress& p) override;

    double profileAlgorithm() override;
  };

} // namespace bgm

#endif // BGM_ASM_ASSEMBLY_TASK_H

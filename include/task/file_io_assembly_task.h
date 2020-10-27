//
// Created by przemek on 12.08.2020.
//

#ifndef OMA_ASSEMBLY_TASK_H
#define OMA_ASSEMBLY_TASK_H

#include <common/assembly_params.h>
#include <mt4cpp.h>
#include <task/abstract_task.h>
#include <utility>

namespace bgm {
  /**
   * Wrapper for assembly pipeline
   */
  class FileIOAssemblyTask : public AbstractTask {
  public:
    using Params = AbstractTask::Params;

    /**
     * Constructor
     * @param inputFilename
     * @param outputFilename
     * @param params parameter for task
     */
    FileIOAssemblyTask(std::string inputFilename,
                       std::string outputFilename,
                       Params params);

  private:
    void operator()(mt4cpp::Progress &p) override;

  protected:
    double profileAlgorithm() override;

    // internal stages
    void loadMaps();
    void writeResultToFile();

    // filenames
    const std::string inputFile_;
    const std::string outputFile_;
  };
} // namespace bgm

#endif // OMA_ASSEMBLY_TASK_H

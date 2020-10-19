//
// Created by przemek on 12.08.2020.
//

#ifndef OMA_ASSEMBLY_TASK_H
#define OMA_ASSEMBLY_TASK_H

#include <algorithms/align_algorithm.h>
#include <algorithms/consensus_builder.h>
#include <common/assembly_params.h>
#include <mt4cpp.h>
#include <utility>

namespace bgm {
  /**
   * Wrapper for assembly pipeline
   */
  class AssemblyTask : mt4cpp::Command {
  public:
    using Aligner   = AlignAlgorithm;
    using Consensus = ConsensusBuilder;
    using Params    = AssemblyParams;

    /**
     * Constructos
     * @param params parameter for task
     */
    explicit AssemblyTask(Params params);

    /**
     * Run task
     * @param verbose true to enable verbose output to cout
     */
    void run(bool verbose = false);

    /**
     * Run task silently with time of aligning, consensus and folding being
     * measured
     * @return time of task run
     */
    double runPerfTest();

  private:
    void operator()(mt4cpp::Progress& p) override;

  protected:
    // worker functions
    void runQuiet();
    void runVerbose();

    // internal stages
    void loadMaps();
    void writeResultToFile();

    // internal phases
    std::function<void(bool)> prepareNextFold_;
    std::function<void()> fold_;

    // params
    Params params_;

    // maps
    MapsContainer maps_;

    // algorithms
    Aligner aligner_;
    Consensus consensus_;
  };
} // namespace bgm

#endif // OMA_ASSEMBLY_TASK_H

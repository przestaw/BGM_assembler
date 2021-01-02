//
// Created by przestaw on 21.10.2020.
//

#ifndef BGM_ASM_ABSTRACT_TASK_H
#define BGM_ASM_ABSTRACT_TASK_H

#include <algorithms/align_algorithm.h>
#include <algorithms/consensus_builder.h>
#include <mt4cpp.h>

namespace bgm {
  /**
   * Base for all more specified assembly tasks
   */
  class AbstractTask : public mt4cpp::Command {
  public:
    using Aligner   = AlignAlgorithm;
    using Consensus = ConsensusBuilder;
    using Params    = AssemblyParams;

    /**
     * Run task silently with time of aligning, consensus and folding being
     * measured
     * @return time of task run
     */
    inline double runPerfTest() { return this->profileAlgorithm(); }

  protected:
    explicit AbstractTask(const Params& params, MapsContainer&& maps)
        : params_(params), aligner_(maps_, params_.getMinOverlap()),
          consensus_(maps_, params_.isMarkerPreference(), !params_.isReuseMaps()) {
      std::swap(maps_, maps);
    }

    virtual void operator()(mt4cpp::Progress& p) = 0;

    virtual double profileAlgorithm() = 0;

    /* * * internal phases * */
    // preapre fold - so maps contain what is desired for next iteration
    inline void prepareNextFold_(bool reuseMaps) {
      if (reuseMaps) {
        auto newMaps = consensus_.getConsensusMapsAndReset();
        maps_.insert(maps_.end(),
                     std::make_move_iterator(newMaps.begin()),
                     std::make_move_iterator(newMaps.end()));
      } else {
        maps_.resetAndReplace(consensus_.getConsensusMaps());
        consensus_.reset();
      }
    }
    // perform algorithm "step"
    inline void fold_() {
      aligner_.alignMapsMulti(params_.getThreads());
      consensus_.runMultiConsensus(params_.getThreads());
    }

    /* * * Members * * */
    // params
    Params params_;
    // maps
    MapsContainer maps_;
    // algorithms
    Aligner aligner_;
    Consensus consensus_;
  };
} // namespace bgm

#endif // BGM_ASM_ABSTRACT_TASK_H

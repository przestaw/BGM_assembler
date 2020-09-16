//
// Created by przemek on 12.08.2020.
//

#ifndef OMA_ASSEMBLY_TASK_H
#define OMA_ASSEMBLY_TASK_H

#include <algorithms/align_algorithm.h>
#include <algorithms/consensus_builder.h>

#include <utility>

namespace oma {
  class AssemblyTask {
  public:
    using Aligner   = AlignAlgorithm;
    using Consensus = ConsensusBuilder;

    class Params {
    public:
      Params(std::string inputFile, std::string outputFile)
          : Params(std::move(inputFile), std::move(outputFile), 1, 200, 0.5,
                   0.1) {}

      Params(std::string inputFile, std::string outputFile,
             unsigned int threads, unsigned int minOverlap, double minPart,
             double threshold, unsigned repeats = 0);

      [[nodiscard]] inline unsigned getThreads() const { return _threads; }
      [[nodiscard]] inline unsigned getMinOverlap() const {
        return _minOverlap;
      }
      [[nodiscard]] inline double getThreshold() const { return _threshold; }
      [[nodiscard]] inline const std::string& getInputFile() const {
        return _inputFile;
      }
      [[nodiscard]] inline const std::string& getOutputFile() const {
        return _outputFile;
      }
      [[nodiscard]] inline double getMinPart() const { return _minPart; }
      [[nodiscard]] inline size_t getMinMapLen() const {
        return 1 + static_cast<size_t>(_minOverlap / _minPart);
      }
      [[nodiscard]] inline unsigned getRepeats() const { return _repeats; }

    private:
      std::string _inputFile;
      std::string _outputFile;
      unsigned _threads;
      unsigned _minOverlap;
      double _minPart;
      double _threshold;
      unsigned _repeats;
    };

    explicit AssemblyTask(Params params);

    void run(bool verbose = false);
    double runPerfTest();

  protected:
    void loadMaps();
    void alignMaps();
    void performConsensus();
    void writeResult();

    void runQuiet();
    void runVerbose();
  private:
    MapsContainer _maps;
    Params _params;
    Aligner _aligner;
    Consensus _consensus;
  };
} // namespace oma

#endif // OMA_ASSEMBLY_TASK_H

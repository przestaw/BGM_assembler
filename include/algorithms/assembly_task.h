//
// Created by przemek on 12.08.2020.
//

#ifndef OMA_ASSEMBLY_TASK_H
#define OMA_ASSEMBLY_TASK_H

#include <algorithms/align_algorithm.h>
#include <algorithms/consensus_builder.h>
#include <utility>

namespace oma {
  /**
   * Wrapper for assembly pipeline
   */
  class AssemblyTask {
  public:
    using Aligner   = AlignAlgorithm;
    using Consensus = ConsensusBuilder;

    /**
     * Params for assembly task
     */
    class Params {
    public:
      /**
       * Simplified default params constructor
       * @param inputFile input file
       * @param outputFile output file
       */
      Params(std::string inputFile, std::string outputFile)
          : Params(std::move(inputFile), std::move(outputFile), 1, 200, 0.5,
                   0.1) {}

      /**
       * Constructor
       * @param inputFile input file
       * @param outputFile output file
       * @param threads no of threads, 0 will enable all available on the
       * machine, other values will create demanded no of threads
       * @param minOverlap minimum overlap length
       * @param minPart minimum overlap part of smaller map
       * @param threshold threshold of differences for overlap to be meaningful
       * @param repeats no of folds to perform before output
       */
      Params(std::string inputFile, std::string outputFile,
             unsigned int threads, unsigned int minOverlap, double minPart,
             double threshold, unsigned repeats = 0);

      /**
       * Getter for desired no of threads
       * @return desired no of threads
       */
      [[nodiscard]] inline unsigned getThreads() const { return _threads; }
      /**
       * Getter for threshold for overlap to be meaningful
       * @return threshold
       */
      [[nodiscard]] inline double getThreshold() const { return _threshold; }
      /**
       * Getter for input filename
       * @return input filename
       */
      [[nodiscard]] inline const std::string& getInputFile() const {
        return _inputFile;
      }
      /**
       * Getter for output filename
       * @return output filename
       */
      [[nodiscard]] inline const std::string& getOutputFile() const {
        return _outputFile;
      }
      /**
       * Getter for minimum map overlap length
       * @return minimum map overlap length
       */
      [[nodiscard]] inline unsigned getMinOverlap() const {
        return _minOverlap;
      }
      /**
       * Getter for minimum map overlap part
       * @return minimum map overlap part
       */
      [[nodiscard]] inline double getMinPart() const { return _minPart; }
      /**
       * Getter for minimum map length for map filtering
       * @return minimum map length
       */
      [[nodiscard]] inline size_t getMinMapLen() const {
        return 1 + static_cast<size_t>(_minOverlap / _minPart);
      }
      /**
       * Getter for no of repeats
       * @return no of repeats
       */
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

  protected:
    // worker functions
    void runQuiet();
    void runVerbose();

    // internal stages
    void loadMaps();
    void alignMaps();
    void performConsensus();
    void writeResult();

    // params
    Params _params;

    // maps
    MapsContainer _maps;

    // algorithms
    Aligner _aligner;
    Consensus _consensus;
  };
} // namespace oma

#endif // OMA_ASSEMBLY_TASK_H

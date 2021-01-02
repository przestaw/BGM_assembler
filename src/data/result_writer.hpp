//
// Created by przestaw on 21.10.2020.
//

#ifndef BGM_ASM_RESULT_WRITER_HPP
#define BGM_ASM_RESULT_WRITER_HPP

#include <data/bin_map.h>
#include <fstream>
#include <string>

namespace bgm {

  class ResultWriter {
  public:
    /**
     * Outputs maps form iterable collection to file
     * @tparam Iterator iterator to the collection holding BinaryMaps
     * @param filename filename
     * @param begin begin iterator with maps
     * @param end end iterator with maps
     * @param predicate filter predicate (default all)
     */
    template <typename Iterator,
              typename = std::enable_if<
                  std::is_same<typename Iterator::value_type, BinaryMap>::value>>
    static void
    writeMapsToFile(const std::string &filename,
                    const Iterator &begin,
                    const Iterator &end,
                    std::function<bool(const BinaryMap &)> predicate = defaultPredicate);

    /**
     * Outputs maps form iterable collection to stream
     * @tparam Iterator iterator to the collection holding BinaryMaps
     * @param output output stream
     * @param begin begin iterator with maps
     * @param end end iterator with maps
     * @param predicate filter predicate (default all)
     */
    template <typename Iterator,
              typename = std::enable_if<
                  std::is_same<typename Iterator::value_type, BinaryMap>::value>>
    static void writeMapsToStream(
        std::ofstream &output,
        const Iterator &begin,
        const Iterator &end,
        std::function<bool(const BinaryMap &)> predicate = defaultPredicate);

    /**
     * Function used as filter predicate for all maps
     * @return true for any map
     */
    static bool defaultPredicate(const bgm::BinaryMap &) { return true; }

    /**
     * Function used as filter predicate for contigs
     * @return true for maps with contigs flag
     */
    static bool contigsPredicate(const bgm::BinaryMap &map) { return map.isContig(); }
  };

  template <typename Iterator, typename>
  void ResultWriter::writeMapsToFile(const std::string &filename,
                                     const Iterator &begin,
                                     const Iterator &end,
                                     std::function<bool(const BinaryMap &)> predicate) {

    // NOTE : result output
    std::ofstream output;
    output.open(filename);

    ResultWriter::writeMapsToStream(output, begin, end, predicate);

    output.close();
  }

  template <typename Iterator, typename>
  void ResultWriter::writeMapsToStream(std::ofstream &output,
                                       const Iterator &begin,
                                       const Iterator &end,
                                       std::function<bool(const BinaryMap &)> predicate) {
    for (auto it = begin; it != end; ++it) {
      if (predicate(*it)) {
        output << (*it).toFastaRecord();
      }
    }
  }

} // namespace bgm

#endif // BGM_ASM_RESULT_WRITER_HPP

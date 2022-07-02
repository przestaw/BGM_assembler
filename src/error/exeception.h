//
// Created by przemek on 18.03.2020.
//

#ifndef TEMPLATE_EXECEPTION_H
#define TEMPLATE_EXECEPTION_H

#include <boost/program_options/errors.hpp>
#include <iostream>
#include <stdexcept>

namespace bgm::error {
  /**
   * Base runtime exception for BGM assembler errors
   */
  class RuntimeException : public std::runtime_error {
  public:
    /**
     * Constructor
     * @param desc error description
     */
    explicit RuntimeException(const std::string &desc) : runtime_error(desc) {}
  };

  /**
   * Exception thrown when a required CLI argument is missing
   */
  class MissingArgument : public boost::program_options::error,
                          public bgm::error::RuntimeException {
  public:
    /**
     * Constructor
     * @param desc missing argument name
     */
    explicit MissingArgument(const std::string &desc)
        : boost::program_options::error("Missing required argument " + desc
                                        + "\nPlease include argument with value"),
          bgm::error::RuntimeException("Missing required argument " + desc
                                       + "\nPlease include argument with value") {}
  };

  /**
   * Exception thrown when an input or output file cannot be opened
   */
  class FileOpenException : public RuntimeException {
  public:
    /**
     * Constructor
     * @param filename path to the file that failed to open
     */
    explicit FileOpenException(const std::string &filename)
        : bgm::error::RuntimeException(
            "File error : " + filename
            + ". Please check if path and filename was provided correctly") {}
  };

  /**
   * Exception thrown when binary maps of incompatible lengths are compared
   */
  class MismatchedArgumentsSize : public RuntimeException {
  public:
    /**
     * Default constructor
     */
    MismatchedArgumentsSize()
        : bgm::error::RuntimeException("Mismatched arguments size") {}
  };
} // namespace bgm::error

#endif // TEMPLATE_EXECEPTION_H

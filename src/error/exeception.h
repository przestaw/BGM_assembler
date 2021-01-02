//
// Created by przemek on 18.03.2020.
//

#ifndef TEMPLATE_EXECEPTION_H
#define TEMPLATE_EXECEPTION_H

#include <boost/program_options/errors.hpp>
#include <iostream>
#include <stdexcept>

namespace bgm::error {
  class RuntimeException : public std::runtime_error {
  public:
    explicit RuntimeException(const std::string &desc) : runtime_error(desc) {}
  };

  class MissingArgument : public boost::program_options::error,
                          public bgm::error::RuntimeException {
  public:
    explicit MissingArgument(const std::string &desc)
        : boost::program_options::error("Missing required argument " + desc
                                        + "\nPlease include argument with value"),
          bgm::error::RuntimeException("Missing required argument " + desc
                                       + "\nPlease include argument with value") {}
  };

  class FileOpenException : public RuntimeException {
  public:
    explicit FileOpenException(const std::string &filename)
        : bgm::error::RuntimeException(
            "File error : " + filename
            + ". Please check if path and filename was provided correctly") {}
  };

  class MismatchedArgumentsSize : public RuntimeException {
  public:
    MismatchedArgumentsSize()
        : bgm::error::RuntimeException("Mismatched arguments size") {}
  };
} // namespace bgm::error

#endif // TEMPLATE_EXECEPTION_H

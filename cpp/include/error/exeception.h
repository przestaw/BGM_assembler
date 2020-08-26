//
// Created by przemek on 18.03.2020.
//

#ifndef TEMPLATE_EXECEPTION_H
#define TEMPLATE_EXECEPTION_H

#include <boost/program_options/errors.hpp>
#include <iostream>
#include <stdexcept>

namespace oma {
  namespace error {
    class exception : public std::runtime_error {
    public:
      explicit exception(const std::string &desc) : runtime_error(desc) {}
    };

    class runtime_exception : public exception {
    public:
      explicit runtime_exception(const std::string &desc) : exception(desc) {}
    };

    class missing_argument : public boost::program_options::error,
                             public oma::error::exception {
    public:
      explicit missing_argument(const std::string &desc)
          : boost::program_options::error(
              "Missing required argument " + desc
              + "\nPlease include argument with value"),
            oma::error::exception("Missing required argument " + desc
                                  + "\nPlease include argument with value") {}
    };
  } // namespace error
} // namespace oma

#endif // TEMPLATE_EXECEPTION_H

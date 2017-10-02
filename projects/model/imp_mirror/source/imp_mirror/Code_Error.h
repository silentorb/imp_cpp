#pragma once

#include <stdexcept>
#include <source_mapping/Source_Point.h>

namespace imp_mirror {

  class Code_Error : public std::runtime_error {
      source_mapping::Source_Range point;

  public:
      Code_Error(const std::string &message, const source_mapping::Source_Range &point) :
        runtime_error(message + ": " + point.to_string()), point(point) {}
  };
}
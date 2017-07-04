#pragma once

#include <stdexcept>
#include <underworld/Source_Point.h>

namespace imp_mirror {

  class Code_Error : public std::runtime_error {
      underworld::Source_Point point;

  public:
      Code_Error(const std::string &message, const underworld::Source_Point &point) :
        runtime_error(message + ": " + point.to_string()), point(point) {}
  };
}
#pragma once

#include "Generic_Parameter.h"

namespace overworld {

  class Function;

  class Function_Interface {
  public:
      virtual const std::vector<Generic_Parameter *> &get_generic_parameters() const = 0;
      virtual Function &get_original() = 0;
  };
}
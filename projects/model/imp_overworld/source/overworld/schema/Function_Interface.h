#pragma once

#include "Generic_Parameter.h"

namespace overworld {

  class Function_Interface {
  public:
      virtual const std::vector<Generic_Parameter_Owner> &get_generic_parameters() const = 0;
  };
}
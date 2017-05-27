#pragma once
#include <underworld/Source_Point.h>
#include "Profession.h"

namespace overworld {

  class Profession_Reference {
  public:
      virtual const Profession &get_profession() = 0;
      virtual void set_profession(const Profession &value) = 0;
      virtual const underworld::Source_Point &get_source_point() = 0;
      virtual const std::string get_name() = 0;
  };
}
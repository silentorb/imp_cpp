#pragma once

#include <underworld/Source_Point.h>
#include "Profession.h"

namespace overworld {

  enum class Element_Type {
      instantiation,
      minion,
      parameter,
      other
  };

  class Profession_Reference {
  public:
      virtual Profession &get_profession() = 0;
      virtual const Profession &get_profession() const = 0;
      virtual void set_profession(Profession &value) = 0;
      virtual const underworld::Source_Point &get_source_point() const = 0;
      virtual const std::string get_name() const = 0;

      virtual Element_Type get_element_type() const {
        return Element_Type::other;
      }

      virtual ~Profession_Reference() {

      }
  };
}
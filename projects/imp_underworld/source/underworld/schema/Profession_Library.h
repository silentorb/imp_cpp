#pragma once

#include "professions.h"

namespace underworld {

  class Profession_Library {
      Unknown unknown;
      Primitive primitives[Primitive_Type_Count];

  public:
      Profession_Library();

      const Unknown &get_unknown() const {
        return unknown;
      }

      const Primitive &get_primitive(Primitive_Type type) const {
        return primitives[static_cast<int>(type)];
      }
  };
}
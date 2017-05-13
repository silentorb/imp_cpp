#pragma once

#include <underworld/schema/Minion.h>
#include <overworld/schema/Minion.h>
#include "Expression.h"

namespace overworld {

  class Minion_Expression : public virtual Expression {
      Minion &minion;

  public:
      Minion_Expression(Minion &minion) : minion(minion) {}

      Type get_type() const override {
        return Type::minion;
      }

      Minion &get_minion() const {
        return this->minion;
      }
  };
}
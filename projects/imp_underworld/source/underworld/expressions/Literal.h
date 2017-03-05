#pragma once

#include <underworld/schema/professions.h>
#include <underworld/schema/Profession_Library.h>
#include "Expression.h"

namespace underworld {

  template<typename T>
  class Literal : public virtual Expression {
      T value;

  public:
      Literal(T value) : value(value) {}

      virtual Profession &get_profession() = 0;
  };

  class Literal_Int : public Literal<int> {
  public:
      Literal_Int(int value) : Literal(value) {}

      Profession &get_profession() override {
        return Profession_Library::get_primitive(Primitive_Type::Int);
      }
  };
}
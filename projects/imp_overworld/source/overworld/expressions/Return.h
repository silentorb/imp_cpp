#pragma once

#include "Expression.h"

namespace overworld {

  class Return : public virtual Expression {
  public:
      Type get_type() override {
        return Type::return_nothing;
      }
  };


  class Return_With_Value : public virtual Expression {
      Expression_Owner value;

  public:
      Return_With_Value(Expression_Owner &value) :
        value(std::move(value)) {}

      Type get_type() override {
        return Type::return_with_value;
      }
  };
}
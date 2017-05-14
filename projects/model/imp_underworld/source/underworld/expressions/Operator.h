#pragma once

#include "Expression.h"

namespace underworld {

  enum class Operator_Type {
      assignment,
  };

  class Operator : public virtual Expression {
      Operator_Type operator_type;

  public:
      Operator(Operator_Type operator_type) : operator_type(operator_type) {}

      Type get_type() const override {
        return Type::Operator;
      }

      const Operator_Type get_operator_type() const {
        return operator_type;
      }
  };
}
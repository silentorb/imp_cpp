#pragma once

#include "Expression.h"

namespace underworld {

  enum class Operator_Type {
      assignment,
  };

  class Operator : public virtual Common_Expression {
      Operator_Type operator_type;

  public:
      Operator(Operator_Type operator_type) : operator_type(operator_type) {}

      virtual ~Operator() {

      }

      Type get_type() const override {
        return Type::Operator;
      }

      const Operator_Type get_operator_type() const {
        return operator_type;
      }

      const Profession &get_profession() const override {
        throw std::runtime_error("Not supported.");
      }

  };
}
#pragma once
#include "Expression.h"

namespace overworld {

  enum class Operator_Type {
      assignment,
  };

  class Operator : public Common_Expression {
      Operator_Type operator_type;

  public:
      Operator(Operator_Type operator_type, const source_mapping::Source_Range &source_range) :
        Common_Expression(source_range),
        operator_type(operator_type) {}

      virtual ~Operator() {

      }

      Type get_type() const override {
        return Type::Operator;
      }

      const Operator_Type get_operator_type() const {
        return operator_type;
      }
  };
}
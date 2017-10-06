#pragma once

#include "Expression.h"
#include "Operator.h"

namespace overworld {

  class Assignment : public Statement {
      Expression_Owner target;
      Operator_Type operator_type;
      Expression_Owner value;

  public:
      Assignment(Expression_Owner &target, Operator_Type operator_type, Expression_Owner &value) :
        target(std::move(target)), operator_type(operator_type), value(std::move(value)) {}

      virtual ~Assignment() {

      }

      Expression_Type get_type() const override {
        return Expression_Type::assignment;
      }

      const Expression_Owner &get_target() const {
        return target;
      }

      Operator_Type get_operator() const {
        return operator_type;
      }

      const Expression_Owner &get_value() const {
        return value;
      }
  };
}
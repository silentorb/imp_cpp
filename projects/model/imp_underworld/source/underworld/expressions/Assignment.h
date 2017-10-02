#pragma once

#include "Expression.h"
#include "Operator.h"

namespace underworld {

  class Assignment : public virtual Statement {
      Expression_Owner target;
      Operator_Type operator_type;
      Expression_Owner value;

  public:
      Assignment(Expression_Owner &target, Operator_Type operator_type, Expression_Owner &value) :
        target(std::move(target)), operator_type(operator_type), value(std::move(value)) {}

      virtual ~Assignment() {

      }

      Type get_type() const override {
        return Type::assignment;
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

      const source_mapping::Source_Range get_source_point() const override {
        throw std::runtime_error("Not supported.");
      }
  };
}
#pragma once

#include "Expression.h"

namespace overworld {

  class Return : public Statement{
  public:
      Expression_Type get_type() const override {
        return Expression_Type::return_nothing;
      }
  };

  class Return_With_Value : public Statement {
      Expression_Owner value;

  public:
      Return_With_Value(Expression_Owner value) :
        value(std::move(value)) {}

      virtual ~Return_With_Value() {

      }

      Expression_Type get_type() const override {
        return Expression_Type::return_with_value;
      }

      const Expression &get_value() const {
        return *value;
      }
  };
}
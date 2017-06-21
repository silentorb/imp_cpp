#pragma once

#include "Expression.h"

namespace overworld {

  class Return : public Statement{
  public:
      Type get_type() const override {
        return Type::return_nothing;
      }
  };


  class Return_With_Value : public Statement {
      Expression_Owner value;

  public:
      Return_With_Value(Expression_Owner value) :
        value(std::move(value)) {}

      Type get_type() const override {
        return Type::return_with_value;
      }

      const Expression &get_value() const {
        return *value;
      }
  };
}
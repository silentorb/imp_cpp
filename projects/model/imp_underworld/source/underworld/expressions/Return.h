#pragma once

#include "Expression.h"

namespace underworld {

  class Return : public virtual Statement {
  public:
      Type get_type() const override {
        return Type::return_nothing;
      }
  };


  class Return_With_Value : public virtual Statement {
      Expression_Owner value;

  public:
      Return_With_Value(Expression_Owner &value) :
        value(std::move(value)) {}

      Type get_type() const override {
        return Type::return_with_value;
      }

      const Expression &get_value() const {
        return *value;
      }

  };
}
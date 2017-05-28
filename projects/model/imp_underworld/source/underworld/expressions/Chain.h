#pragma once

#include "Expression.h"
#include <vector>

namespace underworld {

  class Chain : public virtual Expression {
      std::vector<Expression_Owner> expressions;

  public:
      Type get_type() const override {
        return Type::chain;
      }

      Expression &get_last() override {
        return *expressions[expressions.size() - 1];
      }
  };
}
#pragma once

#include <vector>
#include "overworld/schema/Scope.h"
#include "Expression.h"

namespace overworld {

  class Block : public virtual Expression {
      Scope scope;
      std::vector<Expression_Owner> expressions;

  public:
      Block(const underworld::Block & source) :
        scope(source.get_scope()) {}

      void add_expression(Expression_Owner &expression) {
        expressions.push_back(std::move(expression));
      }

      Scope &get_scope() {
        return scope;
      }

      Type get_type() const override {
        return Type::block;
      }

      const std::vector<Expression_Owner> &get_expressions() const {
        return expressions;
      }
  };
}
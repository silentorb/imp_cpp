#pragma once

#include <vector>
#include "underworld/schema/Scope.h"
#include "Expression.h"

namespace underworld {

  class Block : public virtual Statement {
      Scope scope;
      std::vector<Expression_Owner> expressions;

  public:
      void add_expression(Expression_Owner expression) {
        expressions.push_back(std::move(expression));
      }

      Scope &get_scope() {
        return scope;
      }

      const Scope &get_scope() const {
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
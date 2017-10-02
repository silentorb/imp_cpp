#pragma once

#include <vector>
#include "underworld/schema/Scope.h"
#include "Expression.h"

namespace underworld {

  class Block : public virtual Statement {
      Scope scope;
      std::vector<Expression_Owner> expressions;

  public:
      Block(Scope &parent) : scope(&parent) {}

      virtual ~Block() {

      }

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

      const source_mapping::Source_Point get_source_point() const override {
        throw std::runtime_error("Not supported.");
      }
  };
}
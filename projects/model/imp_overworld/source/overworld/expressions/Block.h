#pragma once

#include <vector>
#include "overworld/schema/Scope.h"
#include "Expression.h"

namespace overworld {

  class Block : public virtual Expression, public virtual Scope_Parent {
      std::vector<Expression_Owner> expressions;

  public:
      void add_expression(Expression_Owner &expression) {
        expressions.push_back(std::move(expression));
      }

      const std::vector<Expression_Owner> &get_expressions() const {
        return expressions;
      }

      virtual Scope &get_scope() = 0;
      virtual const Scope &get_scope() const = 0;

      Type get_type() const override {
        return Type::block;
      }

      Scope_Parent_Type get_scope_parent_type() const override {
        return Scope_Parent_Type::block;
      }
  };


  class Basic_Block : public virtual Block {
      Scope scope;

  public:
      Basic_Block(const underworld::Block &source) :
        scope(source.get_scope(), *this) {}

      virtual Scope &get_scope() override {
        return scope;
      }

      virtual const Scope &get_scope() const override {
        return scope;
      }

  };

  class Function_Block : public virtual Block {
      Function_Scope scope;

  public:
      Function_Block(const underworld::Block &source, Function &function) :
        scope(source.get_scope(), *this, function) {}

      virtual Function_Scope &get_scope() override {
        return scope;
      }

      virtual const Function_Scope &get_scope() const override {
        return scope;
      }

  };
}
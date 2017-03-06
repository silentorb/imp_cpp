#pragma once

#include "Expression.h"
#include "Block.h"

namespace overworld {

  class Conditional_Expression : public virtual Expression {
  protected:
      Expression_Owner condition;
      Expression_Owner expression;

  public:
      Conditional_Expression(Expression_Owner &condition, Expression_Owner &expression) :
        condition(std::move(condition)), expression(std::move(expression)) {}

      Expression &get_condition() {
        return *condition;
      }

      Expression &get_expression() {
        return *expression;
      }
  };

  class If : public Conditional_Expression {
  public:
      If(Expression_Owner &condition, Expression_Owner &expression) :
        Conditional_Expression(condition, expression) {}

      Type get_type() override {
        return Type::If;
      }
  };

  class Else : public virtual Expression {
      Expression_Owner expression;

  public:
      Else(Expression_Owner &expression) :
        expression(std::move(expression)) {}

      Type get_type() override {
        return Type::Else;
      }
  };
}
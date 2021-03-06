#pragma once

#include "Expression.h"
#include "Block_Expression.h"

namespace overworld {

  class Conditional_Expression : public Statement {
  protected:
      Expression_Owner condition;
      Expression_Owner expression;

  public:
      Conditional_Expression(Expression_Owner &condition, Expression_Owner &expression) :
        condition(std::move(condition)), expression(std::move(expression)) {}

      virtual ~Conditional_Expression() {

      }

      const Expression &get_condition() const {
        return *condition;
      }

      const Expression &get_expression() const {
        return *expression;
      }
  };

  class If : public Conditional_Expression {
  public:
      If(Expression_Owner condition, Expression_Owner expression) :
        Conditional_Expression(condition, expression) {}

      Expression_Type get_type() const override {
        return Expression_Type::If;
      }
  };

  class Else : public Expression {
      Expression_Owner expression;

  public:
      Else(Expression_Owner &expression) :
        expression(std::move(expression)) {}

      virtual ~Else() {

      }

      Expression_Type get_type() const override {
        return Expression_Type::Else;
      }
  };
}
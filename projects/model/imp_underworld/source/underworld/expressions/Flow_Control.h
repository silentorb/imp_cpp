#pragma once

#include "Expression.h"
#include "Block.h"

namespace underworld {

  class Conditional_Expression : public Statement {
  protected:
      Expression_Owner condition;
      Expression_Owner expression;

  public:
      Conditional_Expression(Expression_Owner &condition, Expression_Owner &expression) :
        condition(std::move(condition)), expression(std::move(expression)) {}

      virtual ~Conditional_Expression() {

      }

      Expression &get_condition() {
        return *condition;
      }

      Expression &get_expression() {
        return *expression;
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
      If(Expression_Owner &condition, Expression_Owner &expression) :
        Conditional_Expression(condition, expression) {}

      Expression_Type get_type() const override {
        return Expression_Type::If;
      }

      const source_mapping::Source_Range get_source_point() const override {
        throw std::runtime_error("Not supported.");
      }
  };

  class Else : public Expression {
      Expression_Owner expression;
  public:
      virtual ~Else() {

      }

  public:
      Else(Expression_Owner &expression) :
        expression(std::move(expression)) {}

      Expression_Type get_type() const override {
        return Expression_Type::Else;
      }

      const source_mapping::Source_Range get_source_point() const override {
        throw std::runtime_error("Not supported.");
      }
  };
}